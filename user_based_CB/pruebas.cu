#include "scripts.h"
#include <iostream>
#include "structures.h"
#include "distances.h"
#include "cud_sparse_oper.h"
#include "recomender.h"

using namespace std;


int main(int argc, char const *argv[]) {
  int n_ratings, n_users;
  n_ratings = 27753444;
  n_users = 283228;
  // n_of_users("../databases/ml-latest/ratings.csv", n_ratings, n_users, true);
  // cout<<n_ratings<<" "<<n_users<<endl;

  float* values;
  int *row_ind, * col_ind;
  int * ind_users, *row_size;

  float* d_values;
  int *d_row_ind, * d_col_ind;
  int * d_ind_users, * d_row_size;

  d_values = cuda_array<float>(n_ratings);
  d_row_ind = cuda_array<int>(n_ratings);
  d_col_ind = cuda_array<int>(n_ratings);
  d_ind_users = cuda_array<int>(n_users);
  d_row_size = cuda_array<int>(n_users);


  read_ML("../databases/ml-latest/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);

  cuda_H2D<float>(values, d_values, n_ratings);
  cuda_H2D<int>(row_ind, d_row_ind, n_ratings);
  cuda_H2D<int>(col_ind, d_col_ind, n_ratings);
  cuda_H2D<int>(ind_users, d_ind_users, n_users);
  cuda_H2D<int>(row_size, d_row_size, n_users);


  // float* r1 = float_pointer(values, ind_users, 3);
  // int* c1 = int_pointer(col_ind, ind_users, 3);
  //
  // float* r2 = float_pointer(values, ind_users, 1);
  // int* c2 = int_pointer(col_ind, ind_users, 1);
  //
  // cout<<euclidean(r1, c1, row_size[3], r2, c2, row_size[1])<<endl;




  vector<int> ids_movies;
  vector<float> movies_ratings;

  k_recomendations(ids_movies, movies_ratings, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, n_ratings, n_users, EUCLIDEAN, 3242, 10);
  //
  // float* distances = new float[n_users];
  // float* d_distances;
  // d_distances = cuda_array<float>(n_users);
  //
  //
  //
  //
  // int block_size = 256;
  // dim3 block =  dim3(block_size, 1, 1);
  // dim3 grid =  dim3(ceil(n_users / block_size), 1);
  //
  //
  // one2all_pearson<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, 3, n_users);
  // CHECK(cudaDeviceSynchronize());
  // cuda_D2H<float>(d_distances, distances, n_users);
  // CHECK(cudaDeviceSynchronize());
  //
  // cout<<"--------"<<endl;
  // for (size_t i = 0; i < 10; i++) {
  //   cout<<distances[i]<<endl;
  // }
  //
  // float* distances2 = new float[n_users];
  //
  //
  //
  // float v;
  // for (size_t i = 3; i < 4; i++) {
  //   float* r1 = float_pointer(values, ind_users, i);
  //   int* c1 = int_pointer(col_ind, ind_users, i);
  //   // if(i % 1000 == 0)
  //   // cout<<i<<endl;
  //   for (size_t j = 0; j < 10; j++) {
  //     float* r2 = float_pointer(values, ind_users, j);
  //     int* c2 = int_pointer(col_ind, ind_users, j);
  //     v = pearson(r1, c1, row_size[i], r2, c2, row_size[j]);
  //     distances2[j] = v;
  //     // if(v != 0)
  //     //   cout<<v<<endl;
  //
  //   }
  // }
  //
  // cout<<"--------"<<endl;
  // for (size_t i = 0; i < 10; i++) {
  //   cout<<distances[i]<<endl;
  // }

  return 0;
}
