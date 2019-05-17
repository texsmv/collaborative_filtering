#include "scripts.h"
#include "cud_defs.h"
#include "cud_sparse_oper.h"
#include "recomender.h"


int main(int argc, char const *argv[]) {
  int n_ratings, n_users, n_movies, n_ids_movies;
  int n_ratings_20, n_users_20, n_ratings_27, n_users_27, n_movies_27;

  n_ratings_27 = 27753444;
  n_users_27 = 283228;
  n_movies_27 = 53889;


  n_ratings_20 = 20000263;
  n_users_20 = 138493;

  n_ratings = n_ratings_27;
  n_users = n_users_27;
  n_movies = n_movies_27;
  n_ids_movies = 193887;
  // n_ratings
  // n_of_users("../databases/ml-20m/ratings.csv", n_ratings, n_users, true);
  // cout<<n_ratings<<" "<<n_users<<endl;
  float* values;
  int *row_ind, * col_ind;
  int * ind_users, *row_size;
  float* maxs,*mins, *averages;
  int* pos_movies;

  float* item_values;
  int *item_row_ind, * item_col_ind;
  int * ind_items, *item_row_size;



  float* d_values;
  int *d_row_ind, * d_col_ind;
  int * d_ind_users, *d_row_size;
  float* d_maxs,*d_mins, *d_averages;

  float* d_item_values;
  int *d_item_row_ind, * d_item_col_ind;
  int * d_ind_items, *d_item_row_size;

  d_values = cuda_array<float>(n_ratings);
  d_row_ind = cuda_array<int>(n_ratings);
  d_col_ind = cuda_array<int>(n_ratings);
  d_ind_users = cuda_array<int>(n_users);
  d_row_size = cuda_array<int>(n_users);
  d_maxs = cuda_array<float>(n_users);
  d_mins = cuda_array<float>(n_users);
  d_averages = cuda_array<float>(n_users);


  d_item_values = cuda_array<float>(n_ratings);
  d_item_row_ind = cuda_array<int>(n_ratings);
  d_item_col_ind = cuda_array<int>(n_ratings);
  d_ind_items = cuda_array<int>(n_movies);
  d_item_row_size = cuda_array<int>(n_movies);


  string path = "../databases/ml-latest/ratings.csv";

  read_ML_ratings( path, n_ratings,  n_users, true  , values,row_ind, col_ind, ind_users, row_size,"27");
  read_ML_ratings_items(path, n_ratings, n_users, n_movies, n_ids_movies, true,  item_values,  item_row_ind,  item_col_ind,  ind_items, item_row_size, "27", pos_movies);
  average_per_user(values,ind_users, row_size,maxs,mins,averages,n_users);

  cuda_H2D<float>(values, d_values, n_ratings);
  cuda_H2D<int>(row_ind, d_row_ind, n_ratings);
  cuda_H2D<int>(col_ind, d_col_ind, n_ratings);
  cuda_H2D<int>(ind_users, d_ind_users, n_users);
  cuda_H2D<int>(row_size, d_row_size, n_users);
  cuda_H2D<float>(maxs, d_maxs, n_users);
  cuda_H2D<float>(mins, d_mins, n_users);
  cuda_H2D<float>(averages, d_averages, n_users);

  cuda_H2D<float>(item_values, d_item_values, n_ratings);
  cuda_H2D<int>(item_row_ind, d_item_row_ind, n_ratings);
  cuda_H2D<int>(item_col_ind, d_item_col_ind, n_ratings);
  cuda_H2D<int>(ind_items, d_ind_items, n_movies);
  cuda_H2D<int>(item_row_size, d_item_row_size, n_movies);

  float* similarity_matrix;

  float* posicion_sm = new float(0);
  if(!fexists("binary_files/posicion_sm")){
    cout<<"writing sm pos"<<endl;
    write_array<float>(posicion_sm, 1, "binary_files/posicion_sm");
  }
  else{
    read_array<float>(posicion_sm, 1, "binary_files/posicion_sm");
    cout<<"reading sm pos"<<endl;
  }
  cout<<"Posicion actual: "<<*posicion_sm<<endl;


  get_similarity_matrix(n_ratings, n_users, n_movies, d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, d_averages, similarity_matrix, posicion_sm);

  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0;  j< 4; j++) {
      /* code */
      cout<<get_tm2(i, j, similarity_matrix)<<" ";
    }
    cout<<endl;
  }

    /* code */
  float* r1 = float_pointer(values, ind_users, 3);
  int* c1 = int_pointer(col_ind, ind_users, 3);
  cout<<predecir(similarity_matrix, maxs, mins, r1, c1, row_size[3], pos_movies[169], 3, pos_movies)<<endl;
  cout<<predecir(similarity_matrix, maxs, mins, r1, c1, row_size[3], pos_movies[339], 3, pos_movies)<<endl;
  cout<<predecir(similarity_matrix, maxs, mins, r1, c1, row_size[3], pos_movies[349], 3, pos_movies)<<endl;
  cout<<predecir(similarity_matrix, maxs, mins, r1, c1, row_size[3], pos_movies[296], 3, pos_movies)<<endl;

  // for (size_t i = 0; i < 100 ; i++) {
  //   /* code */
  //   cout<<pos_movies[i]<<endl;
  //   cout<<predecir(similarity_matrix, maxs, mins, r1, c1, row_size[3], pos_movies[i], 3, pos_movies)<<endl;
  // }

  // cout<<get_tm(0, 0, similarity_matrix)<<endl;
  // cout<<get_tm(0, 1, similarity_matrix)<<endl;
  // cout<<get_tm(0, 2, similarity_matrix)<<endl;
  // cout<<get_tm(123, 123, similarity_matrix)<<endl;


  // int tam = int(((double)n_movies + 1) * (double)n_movies / 2);
  // similarity_matrix = new float[tam];
  // for (size_t i = 0; i < tam; i++) {
  //   similarity_matrix[i] = 1;
  // }
  // write_array<float>(similarity_matrix, tam, "prueba");



  // float block_size = 256;
  // dim3 block =  dim3(block_size, 1, 1);
  // dim3 grid =  dim3(ceil(n_movies / block_size), 1);
  //
  // float* distances;
  // distances = new float[n_movies];
  // float* d_distances = cuda_array<float>(n_movies);
  // int pos_movie = 0;
  // one2all_adjusted_cosine<<<grid, block>>>(d_averages, d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, d_distances, pos_movie, n_movies);
  // CHECK(cudaDeviceSynchronize());
  // cuda_D2H<float>(d_distances, distances, n_movies);
  // CHECK(cudaDeviceSynchronize());
  //
  // for (size_t i = 0; i < 300; i++) {
  //   cout<<distances[i]<<endl;
  // }

  // int N = 5;
  // int tam = (N + 1) * N / 2;
  // cout<<tam<<endl;
  // float t[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  // float* matriz = &t[0];
  //
  // set_tm(4,2,100, matriz);
  // cout<<get_tm(0,0, matriz)<<endl;
  // cout<<get_tm(1,4, matriz)<<endl;
  // cout<<get_tm(3,4, matriz)<<endl;
  // cout<<get_tm(4,2, matriz)<<endl;


  // for (size_t i = 0; i < n_users; i++) {
  //   cout<<maxs[i]<<endl;
  //   cout<<mins[i]<<endl;
  //   cout<<averages[i]<<endl;
  // }

  // for (size_t i = 0; i < 40; i++) {
  //   cout<<item_values[i]<<endl;
  //   cout<<item_row_ind[i]<<endl;
  //   cout<<item_col_ind[i]<<endl;
  //   cout<<ind_items[i]<<endl;
  //   cout<<item_row_size[i]<<endl;
  //   cout<<endl;
  // }


  return 0;
}
