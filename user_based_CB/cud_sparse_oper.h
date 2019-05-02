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

__global__ void one2all_euclidean(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* d_distances, int pos_user, int n_users){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users){
    float* r1 = float_pointer(d_values, d_ind_users, pos_user);
    int* c1 = int_pointer(d_col_ind, d_ind_users, pos_user);
    float* r2 = float_pointer(d_values, d_ind_users, i);
    int* c2 = int_pointer(d_col_ind, d_ind_users, i);
    d_distances[i] = d_euclidean(r1, c1, d_row_size[pos_user], r2, c2, d_row_size[i]);
  }
}

__global__ void one2all_pearson(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* d_distances, int pos_user, int n_users){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users){
    float* r1 = float_pointer(d_values, d_ind_users, pos_user);
    int* c1 = int_pointer(d_col_ind, d_ind_users, pos_user);
    float* r2 = float_pointer(d_values, d_ind_users, i);
    int* c2 = int_pointer(d_col_ind, d_ind_users, i);
    d_distances[i] = d_pearson(r1, c1, d_row_size[pos_user], r2, c2, d_row_size[i]);
  }
}

/*
distances es inicializado aqui y almacena las distancias de todos
contra el usuario en la posicion pos_user
todas las demas variables se asumen que estan inicializadas
*/
void distances_one2all(float*& distances, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, int n_users, int measure, int pos_user){
  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);

  distances = new float[n_users];
  float* d_distances = cuda_array<float>(n_users);
  switch (measure) {
    case EUCLIDEAN: one2all_euclidean<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, pos_user, n_users);
      break;
    case PEARSON: one2all_pearson<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, pos_user, n_users);
      break;
  }
  CHECK(cudaDeviceSynchronize());
  cuda_D2H<float>(d_distances, distances, n_users);
  CHECK(cudaDeviceSynchronize());

}



#endif
