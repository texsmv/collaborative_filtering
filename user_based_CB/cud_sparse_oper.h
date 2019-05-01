#ifndef SPARSE_OPERATIONS_H
#define SPARSE_OPERATIONS_H

#include <bits/stdc++.h>
#include <fstream>
#include "cud_defs.h"
#include <limits>
#include <math.h>

using namespace std;

// #define n_ratings 27753444
// #define n_users 283228
#define n_ratings 20000263
#define n_users 138493
// #define n_movies 193886
#define n_movies 131262

#define CUDART_INF_F __int_as_float(0x7f800000)
#define n_knn 5

typedef pair<float, int> par;




__host__ __device__ float* float_pointer(float* values, int* ind_users, int id_user){
  return &(values[ind_users[id_user]]);
}

__host__ __device__ int* int_pointer(int* values, int* ind_users, int id_user){
  return &(values[ind_users[id_user]]);
}





__device__ float d_cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float lxl = 0;
  float lyl = 0;

  int it1 = 0;
  int it2 = 0;
  // int n = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        lxl+= pow(r1[it1],2);
        lyl+= pow(r2[it2],2);
        it1++; it2++;
         // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  return (lxl == 0 || lyl == 0)? CUDART_INF_F : (xy / (sqrt(lxl)*sqrt(lyl)));
}

__host__ float cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float lxl = 0;
  float lyl = 0;

  int it1 = 0;
  int it2 = 0;
  // int n = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        lxl+= pow(r1[it1],2);
        lyl+= pow(r2[it2],2);
        it1++; it2++;
         // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  return (lxl == 0 || lyl == 0)? numeric_limits<float>::infinity() : (xy / (sqrt(lxl)*sqrt(lyl)));
}


__device__ float d_pearson(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float x = 0;
  float y = 0;
  float y_2 = 0;
  float x_2 = 0;
  int n = 0;

  int it1 = 0;
  int it2 = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        x += r1[it1];
        y += r2[it2];
        x_2 += (r1[it1]) * (r1[it1]);
        y_2 += (r2[it2]) * (r2[it2]);
        it1++; it2++;
        n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  float n1 = xy - (( x * y )/n);
  float d1 = sqrt(x_2 -((x*x)/n));
  float d2 = sqrt(y_2 -((y*y)/n));
  return (n == 0) ? 0: n1 / (d1 * d2);
}

__host__ float pearson(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float x = 0;
  float y = 0;
  float y_2 = 0;
  float x_2 = 0;
  int n = 0;

  int it1 = 0;
  int it2 = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        x += r1[it1];
        y += r2[it2];
        x_2 += pow(r1[it1],2);
        y_2 += pow(r2[it2],2);
        it1++; it2++;
        n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  float n1 = xy - (( x * y )/n);
  float d1 = sqrt(x_2 -(pow(x,2)/n));
  float d2 = sqrt(y_2 -(pow(y,2)/n));
  return (n == 0) ? 0 : n1 / (d1 * d2);
}




__device__ float d_euclidean(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;
  while (it1 < s1 && it2 < s2) {
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
  while (it1 < s1 && it2 < s2) {
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


__global__ void one2all_p(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* d_distances, int pos_user){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users){
    float* r1 = float_pointer(d_values, d_ind_users, pos_user);
    int* c1 = int_pointer(d_col_ind, d_ind_users, pos_user);
    float* r2 = float_pointer(d_values, d_ind_users, i);
    int* c2 = int_pointer(d_col_ind, d_ind_users, i);
    d_distances[i] = d_pearson(r1, c1, d_row_size[pos_user], r2, c2, d_row_size[i]);


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



void knn2(float* distances, int* ids, float* dists, int k){
  // ids = new int[k];
  // dists = new float[k];
  // par* p;
  priority_queue<par, vector<par>, greater<par> > pq;
  for (size_t i = 0; i < n_users; i++) {
    pq.push(make_pair(distances[i], i));
  }
  for (size_t i = 0; i < k; i++) {
    par p = pq.top();
    ids[i] = p.second;
    dists[i] = p.first;
    pq.pop();
  }


}



vector<int> top_k_movies(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int id_user, int k){
  cout<<"k movies"<<endl;
  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);
  vector<int> k_movies;
  int* ids = new int[n_knn];
  float* dists = new float[n_knn];

  float* distances = new float[n_users];
  float* d_distances = cuda_array<float>(n_users);
  for (size_t i = 0; i < n_knn; i++) {
    cout<<"id: "<<ids[i]<<"distancias: "<<dists[i]<<endl;
  }

  one2all<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, id_user);
  CHECK(cudaDeviceSynchronize());
  cuda_D2H<float>(d_distances, distances, n_users);
  CHECK(cudaDeviceSynchronize());

  knn2(distances, ids, dists, n_knn);



  for (size_t i = 0; i < n_knn; i++) {
    priority_queue<par, vector<par>, greater<par> > pq;
    float* vals = float_pointer(values, ind_users, ids[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, ids[i]);
    for (size_t j = 0; j < row_size[ids[i]]; j++) {
      pq.push(make_pair(vals[j], ids_movies[j]));
    }
    // cout<<endl;
    par pelicula = pq.top();
    k_movies.push_back(pelicula.second);
  }
  cout<<"end movies"<<endl;
  return k_movies;
}


float proyeccion_movies(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int id_user, int id_pelicula){
  cout<<"k movies"<<endl;
  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);
  vector<int> k_movies;
  int* ids = new int[n_knn];
  float* dists = new float[n_knn];

  float* distances = new float[n_users];
  float* d_distances = cuda_array<float>(n_users);
  one2all_p<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, id_user);
  CHECK(cudaDeviceSynchronize());
  cuda_D2H<float>(d_distances, distances, n_users);
  CHECK(cudaDeviceSynchronize());

  knn2(distances, ids, dists, n_knn);

  // for (size_t i = 0; i < n_knn; i++) {
  //   cout<<ids[i]<<" - "<<dists[i]<<endl;
  // }
  //
  // for (size_t i = 0; i < n_users; i++) {
  //   cout<<"RS: "<<i<< " - "<<row_size[i]<<endl;
  // }

  // n_knns(dists, ids, id_user, 1, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size);
  vector<int> ids_s;
  vector<float> distances_s;
  vector<float> valoraciones_s;

  float sum = 0;
  int n = 0;
  for (size_t i = 0; i < n_knn; i++) {
    float* vals = float_pointer(values, ind_users, ids[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, ids[i]);
    // cout<<"i: "<<i<<" "<< ids[i]<<" of size: "<<row_size[ids[i]]<<endl;

    for (size_t j = 0; j < row_size[ids[i]]; j++) {
      if(ids_movies[j] == id_pelicula){
        distances_s.push_back(distances[i]);
        valoraciones_s.push_back(vals[j]);
        ids_s.push_back(ids[i]);
        sum += vals[j]; n++;
        break;
      }
    }
  }

  float recomendado = 0;
  cout<<"Suma: "<<sum<<" - "<<n;

  for (size_t i = 0; i < ids_s.size(); i++) {
    // cout<<distances_s[i]<<"  "<<valoraciones_s[i]<<endl;
    recomendado += (distances_s[i] / sum) * valoraciones_s[i];
  }

  cout<<"end movies"<<endl;
  if(n == 0){
    return 0;
  }
  return recomendado;
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
    // if(i > 4000000){
    //   cout<<line<<" "<<i<<endl;
    //
    // }
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
