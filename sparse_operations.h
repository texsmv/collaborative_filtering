#ifndef SPARSE_OPERATIONS_H
#define SPARSE_OPERATIONS_H

#include <bits/stdc++.h>
#include <fstream>
#include "cuda_functions.h"
#include <limits>


using namespace std;

#define n_ratings 27753444
#define n_users 283228
#define n_movies 193886
#define CUDART_INF_F __int_as_float(0x7f800000)
#define n_knn 1000

typedef pair<float, int> par;




__host__ __device__ float* float_pointer(float* values, int* ind_users, int id_user){
  return &(values[ind_users[id_user]]);
}

__host__ __device__ int* int_pointer(int* values, int* ind_users, int id_user){
  return &(values[ind_users[id_user]]);
}


__device__ float d_euclidean(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;
  while (it1 < s1 && it2 < 2) {
    if(col1[it1] == col2[it2]){
      sum += (r1[it1] - r2[it2]) * (r1[it1] - r2[it2]);
      it1++; it2++; n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  if(n == 0)
    return CUDART_INF_F;
  return sqrt(sum);
}

__host__ float euclidean(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;
  while (it1 < s1 && it2 < 2) {
    if(col1[it1] == col2[it2]){
      sum += (r1[it1] - r2[it2]) * (r1[it1] - r2[it2]);
      it1++; it2++; n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  if(n == 0)
    return numeric_limits<float>::infinity();
  return sqrt(sum);
}








__global__ void one2all(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* d_distances, int pos_user){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users){
    float* r1 = float_pointer(d_values, d_ind_users, pos_user);
    int* c1 = int_pointer(d_col_ind, d_ind_users, pos_user);
    float* r2 = float_pointer(d_values, d_ind_users, i);
    int* c2 = int_pointer(d_col_ind, d_ind_users, i);
    d_distances[i] = d_euclidean(r1, c1, d_row_size[pos_user], r2, c2, d_row_size[i]);


  }

}


void knn(float* distances, int* ids, float* dists, int pos, int k){
  // ids = new int[k];
  // dists = new float[k];
  // par* p;
  priority_queue<par, vector<par>, greater<par> > pq;
  for (size_t i = 0; i < n_users; i++) {
    pq.push(make_pair(distances[i], i));
  }
  for (size_t i = 0; i < k; i++) {
    par p = pq.top();
    ids[pos * k + i] = p.second;
    dists[pos * k + i] = p.first;
    pq.pop();
  }


}



void n_knns(float*& dists, int*& ids, int b, int n, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size){
  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);

  dists = new float[n * n_knn];
  ids = new int[n * n_knn];

  float* distances = new float[n_users];
  float* d_distances = cuda_array<float>(n_users);
  for (size_t i = b; i < b + n; i++) {
    one2all<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, i);
    CHECK(cudaDeviceSynchronize());
    cuda_D2H<float>(d_distances, distances, n_users);
    CHECK(cudaDeviceSynchronize());
    knn(distances, ids, dists, i, n_knn);
  }
}









vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


void read_movieLens(string nombre, float*& values, int*& row_ind, int*& col_ind, int*& ind_users, int*& row_size){

  values = new float[n_ratings];
  row_ind = new int[n_ratings];
  col_ind = new int[n_ratings];
  ind_users = new int[n_users];
  row_size = new int[n_users];


  std::ifstream infile(nombre);
  std::string line;
  std::getline(infile, line);


  int id_pelicula_maximo = 0;
  int id_usuario_maximo = 0;
  int n_r = -1;

  int i = 0;
  while (std::getline(infile, line)) {
    if(i % 1000000 == 0)
      cout<<i<<endl;
    std::istringstream iss(line);
    vector<string> tokens = split(line, ',');
    if(id_pelicula_maximo < atoi(tokens[1].c_str()))
      id_pelicula_maximo = atoi(tokens[1].c_str());

    if(id_usuario_maximo < atoi(tokens[0].c_str())){
      if(n_r != -1)
        row_size[id_usuario_maximo - 1] = n_r;
      n_r = 0;
      ind_users[id_usuario_maximo] = i;
      id_usuario_maximo = atoi(tokens[0].c_str());
    }

    row_ind[i] = atoi(tokens[0].c_str());
    col_ind[i] = atoi(tokens[1].c_str());
    values[i] = atof(tokens[2].c_str());
    n_r++;
    i++;
  }
  row_size[n_users - 1] = n_r;


  cout<<"Total de ratings: "<<i<<endl;
  cout<<"Id de usuario maximo: "<<id_usuario_maximo<<endl;
  cout<<"Id de pelicula maximo: "<<id_pelicula_maximo<<endl;
  // cout<<"Id maximo de peliculas: "<<id_pel_max<<endl;
  infile.close();
}









#endif
