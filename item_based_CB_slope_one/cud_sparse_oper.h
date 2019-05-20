#ifndef SPARSE_OPERATIONS_H
#define SPARSE_OPERATIONS_H
#include <math.h>
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

unsigned int get_tm2(int i, int j, unsigned int* matriz){
  if(i < j){
    int temp = j;
    j = i;
    i = temp;
    int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
    unsigned int val = matriz[pos];
    int card = val/10000;
    int desv = val - (card * 10000);
    desv = (1000 - desv);
    return desv + (card * 10000);
  }
  else{
    int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
    return matriz[pos];
  }
  // cout<<"posicion "<<pos<<endl;
}

void set_tm2(int i, int j, unsigned int val, unsigned int* matriz){
  if(i < j){
    // int temp = j;
    // j = i;
    // i = temp;
    // int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
    // matriz[pos] = -val;
    // int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
    // matriz[pos] = val;
  }
  else{
    int pos = int(((double)i + 1) * (double)i / 2 + (double)j);
    matriz[pos] = val;
  }
  // cout<<"posicion "<<pos<<endl;
}

__global__ void one2all_desviacion(float* d_item_values, int* d_item_row_ind, int* d_item_col_ind, int* d_ind_items, int* d_item_row_size, unsigned int* d_distances, int pos_movie, int n_movies){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_movies){
    float* r1 = float_pointer(d_item_values, d_ind_items, pos_movie);
    int* c1 = int_pointer(d_item_col_ind, d_ind_items, pos_movie);
    float* r2 = float_pointer(d_item_values, d_ind_items, i);
    int* c2 = int_pointer(d_item_col_ind, d_ind_items, i);
    d_distances[i] = d_desviacion(r1, c1, d_item_row_size[pos_movie], r2, c2, d_item_row_size[i]);
  }
}

void get_similarity_matrix(int n_ratings, int n_users, int n_movies, float*& d_item_values, int*& d_item_row_ind, int*& d_item_col_ind, int*& d_ind_items, int*& d_item_row_size, unsigned int*& similarity_matrix, float*& posicion_sm){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_movies / block_size), 1);
  unsigned int* distances = new unsigned int[n_movies];
  unsigned int* d_distances = cuda_array<unsigned int>(n_movies);
  cout<<"creando array"<<endl;
  int tam = int(((double)n_movies + 1) * (double)n_movies / 2);
  similarity_matrix = new unsigned int[tam];
  if(fexists("binary_files/similarity_matrix_desviacion")){
    read_array<unsigned int>(similarity_matrix, tam, "binary_files/similarity_matrix_desviacion");
    // for (size_t i = 0; i < 20; i++) {
    //   for (size_t j = 0; j < 20; j++) {
    //     cout<<get_tm2(j, i, similarity_matrix)<<" ";
    //   }
    //   cout<<endl;
    // }

  }

  cout<<"array creado"<<endl;
  cout<<tam<<endl;

  reloj r;
  int counter = 0;
  for (size_t i = *posicion_sm; i < n_movies; i++) {
    cout<<i<<endl;
    r.start();
    one2all_desviacion<<<grid, block>>>(d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, d_distances, i, n_movies);
    CHECK(cudaDeviceSynchronize());
    cuda_D2H<unsigned int>(d_distances, distances, n_movies);
    CHECK(cudaDeviceSynchronize());
    for (size_t j = 0; j < n_movies; j++) {
      set_tm2(i, j, distances[j], similarity_matrix);
    }
    r.stop();
    cout<<(r.time())<<"ms"<<endl;
    counter++;
    if(counter % 20000 == 0){
      counter = 0;
      (*posicion_sm) = i;

      r.start();
      write_array<float>(posicion_sm, 1, "binary_files/posicion_sm");
      write_array<unsigned int>(similarity_matrix, tam, "binary_files/similarity_matrix_desviacion");
      r.stop();
      cout<<(r.time())<<"ms - writing"<<endl;
    }
  }
  if((*posicion_sm) != n_movies){
    (*posicion_sm) = n_movies;
    write_array<float>(posicion_sm, 1, "binary_files/posicion_sm");
    write_array<unsigned int>(similarity_matrix, tam, "binary_files/similarity_matrix_desviacion");
  }

}

#endif
