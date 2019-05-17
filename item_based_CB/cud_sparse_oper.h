#ifndef SPARSE_OPERATIONS_H
#define SPARSE_OPERATIONS_H

#include <bits/stdc++.h>
#include <fstream>
#include "cud_defs.h"
#include <limits>
#include <math.h>
#include "distances.h"
#include "structures.h"

using namespace std;

#define n_knn 5

typedef pair<float, int> par;


float get_tm(int i, int j, float* matriz){
  if(i < j){
    int temp = j;
    j = i;
    i = temp;
  }
  int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
  // cout<<"posicion "<<pos<<endl;
  return matriz[pos];
}

void set_tm(int i, int j, float val, float* matriz){
  if(i < j){
    int temp = j;
    j = i;
    i = temp;
  }
  int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
  // cout<<"posicion "<<pos<<endl;
  matriz[pos] = val;
}

float get_tm2(int i, int j, float* matriz){
  if(i < j){
    int temp = j;
    j = i;
    i = temp;
  }
  int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
  // cout<<"posicion "<<pos<<endl;
  return matriz[pos];
}

void set_tm2(int i, int j, float val, float* matriz){
  if(i < j){
    int temp = j;
    j = i;
    i = temp;
  }
  int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
  // cout<<"posicion "<<pos<<endl;
  matriz[pos] = val;
}

__global__ void one2all_adjusted_cosine(float* d_averages, float* d_item_values, int* d_item_row_ind, int* d_item_col_ind, int* d_ind_items, int* d_item_row_size, float* d_distances, int pos_movie, int n_movies){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_movies){
    float* r1 = float_pointer(d_item_values, d_ind_items, pos_movie);
    int* c1 = int_pointer(d_item_col_ind, d_ind_items, pos_movie);
    float* r2 = float_pointer(d_item_values, d_ind_items, i);
    int* c2 = int_pointer(d_item_col_ind, d_ind_items, i);
    d_distances[i] = d_cosine2(r1, c1, d_item_row_size[pos_movie], r2, c2, d_item_row_size[i], d_averages);
  }
}

void get_similarity_matrix(int n_ratings, int n_users, int n_movies, float*& d_item_values, int*& d_item_row_ind, int*& d_item_col_ind, int*& d_ind_items, int*& d_item_row_size, float*& d_averages, float*& similarity_matrix, float*& posicion_sm){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_movies / block_size), 1);
  float* distances = new float[n_movies];
  float* d_distances = cuda_array<float>(n_movies);
  cout<<"creando array"<<endl;
  int tam = int(((double)n_movies + 1) * (double)n_movies / 2);
  similarity_matrix = new float[tam];
  if(fexists("binary_files/similarity_matrix_cosine")){
    read_array<float>(similarity_matrix, tam, "binary_files/similarity_matrix_cosine");
    // for (size_t i = 0; i < 305; i++) {
    //   cout<<similarity_matrix[i]<<" i: "<<i<<endl;
    // }
  }

  cout<<"array creado"<<endl;
  cout<<tam<<endl;

  reloj r;
  int counter = 0;
  for (size_t i = *posicion_sm; i < n_movies; i++) {
    cout<<i<<endl;
    r.start();
    one2all_adjusted_cosine<<<grid, block>>>(d_averages, d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, d_distances, i, n_movies);
    CHECK(cudaDeviceSynchronize());
    cuda_D2H<float>(d_distances, distances, n_movies);
    CHECK(cudaDeviceSynchronize());
    for (size_t j = 0; j < n_movies; j++) {
      set_tm(i, j, distances[j], similarity_matrix);
    }
    r.stop();
    cout<<(r.time())<<"ms"<<endl;
    counter++;
    if(counter % 5000 == 0){
      counter = 0;
      (*posicion_sm) = i;

      r.start();
      write_array<float>(posicion_sm, 1, "binary_files/posicion_sm");
      write_array<float>(similarity_matrix, tam, "binary_files/similarity_matrix_cosine");
      r.stop();
      cout<<(r.time())<<"ms - writing"<<endl;
    }
  }
  if((*posicion_sm) != n_movies){
    (*posicion_sm) = n_movies;
    write_array<float>(posicion_sm, 1, "binary_files/posicion_sm");
    write_array<float>(similarity_matrix, tam, "binary_files/similarity_matrix_cosine");
  }

}

#endif
