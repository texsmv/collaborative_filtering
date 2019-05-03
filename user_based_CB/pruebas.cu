#include "scripts.h"
#include <iostream>
#include "structures.h"
#include "distances.h"
#include "cud_sparse_oper.h"
#include "recomender.h"


using namespace std;


int main(int argc, char const *argv[]) {




  int n_ratings, n_users;
  int n_ratings_20, n_users_20, n_ratings_27, n_users_27;

  n_ratings_27 = 27753444;
  n_users_27 = 283228;

  n_ratings_20 = 20000263;
  n_users_20 = 138493;

  n_ratings = n_ratings_20;
  n_users = n_users_20;
  // n_ratings
  // n_of_users("../databases/ml-20m/ratings.csv", n_ratings, n_users, true);
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


  read_ML("../databases/ml-20m/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);

  cuda_H2D<float>(values, d_values, n_ratings);
  cuda_H2D<int>(row_ind, d_row_ind, n_ratings);
  cuda_H2D<int>(col_ind, d_col_ind, n_ratings);
  cuda_H2D<int>(ind_users, d_ind_users, n_users);
  cuda_H2D<int>(row_size, d_row_size, n_users);


  // float* r1 = float_pointer(values, ind_users, 283227);
  // int* c1 = int_pointer(col_ind, ind_users, 283227);
  // cout<<"tam: "<<row_size[283227]<<endl;
  // for (size_t i = 0 ; i < row_size[283227]; i++) {
  //   cout<<i<<" movie: "<<c1[i]<<" - "<<r1[i]<<endl;
  // }



  // float* r1 = float_pointer(values, ind_users, 3);
  // int* c1 = int_pointer(col_ind, ind_users, 3);
  //
  // float* r2 = float_pointer(values, ind_users, 1);
  // int* c2 = int_pointer(col_ind, ind_users, 1);
  //
  // cout<<euclidean(r1, c1, row_size[3], r2, c2, row_size[1])<<endl;


  int pos;
  int id_movie;
  while (true) {
    cin>>pos;
    cin>>id_movie;
    vector<int> ids_movies;
    vector<float> movies_ratings;
    reloj r;
    r.start();
    // k_proyection(ids_movies, movies_ratings, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, n_ratings, n_users,COSINE, pos, id_movie,n_users);
    k_recomendations(ids_movies, movies_ratings, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, n_ratings, n_users, COSINE, pos, 100);
    r.stop();
    cout<<r.time()<<"ms"<<endl;
    for (size_t i = 0; i < ids_movies.size(); i++) {
      cout<<ids_movies[i]<<" "<<movies_ratings[i]<<endl;
    }
  }

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
  //   cout<<i<<endl;
  //   float* r1 = float_pointer(values, ind_users, i);
  //   int* c1 = int_pointer(col_ind, ind_users, i);
  //   // if(i % 1000 == 0)
  //   // cout<<i<<endl;
  //   for (size_t k = 0; k < row_size[i]; k++) {
  //     cout<<r1[k]<<" -- "<<c1[k]<<endl;
  //   }
  //   cout<<"------------------"<<endl;
  //   cout<<"------------------"<<endl;
  //   cout<<"------------------"<<endl;
  //   for (size_t j = 283227; j < n_users; j++) {
  //     cout<<"j: "<<j<<endl;
  //     float* r2 = float_pointer(values, ind_users, j);
  //     int* c2 = int_pointer(col_ind, ind_users, j);
  //     if(j == 283227){
  //         for (size_t k = 0; k < row_size[j]; k++) {
  //           cout<<r2[k]<<" -- "<<c2[k]<<endl;
  //         }
  //     }
  //
  //     cout<<"-"<<endl;
  //     v = cosine(r1, c1, row_size[i], r2, c2, row_size[j]);
  //     cout<<"-"<<endl;
  //     if(std::isnan(v)){
  //       cout<<"nan"<<endl;
  //     }
  //     distances2[j] = v;
  //     cout<<"-"<<endl;
  //     // if(v != 0)
  //     //   cout<<v<<endl;
  //
  //   }
  // }

  // cout<<"--------"<<endl;
  // for (size_t i = 0; i < 200; i++) {
  //   if (distances2[i] != 0)
  //     cout<<distances2[i]<<endl;
  // }
  //
  return 0;
}
