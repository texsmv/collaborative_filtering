#ifndef SPARSE_OPERATIONS_H
#define SPARSE_OPERATIONS_H

#include <fstream>
#include "cud_defs.h"

#include "structures.h"
#include <set>


template<class T>
void initialize_arr(T* arr, int n, T val){
  for (size_t i = 0; i < n; i++) {
    arr[i] = val;
  }
}

__global__ void pearson_x_item(float* d_item_values, int* d_ind_items, int* d_item_col_ind, float* d_sum_xy, float* d_sum_x, float* d_sum_y, float* d_sum_x2, float* d_sum_y2, int* d_n, bool* d_b_dists, float val, int n_users_x_movie, int id_movie){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users_x_movie){
    float* r1 = float_pointer(d_item_values, d_ind_items, id_movie);
    int* c1 = int_pointer(d_item_col_ind, d_ind_items, id_movie);
    // d_distances[c1[i]] += abs(r1[i] - val);
    d_sum_xy[c1[i]] += (val * r1[i]);
    d_sum_x[c1[i]] += val;
    d_sum_y[c1[i]] += r1[i];
    d_sum_x2[c1[i]] += (val * val);
    d_sum_y2[c1[i]] += (r1[i] * r1[i]);
    d_n[c1[i]] = d_n[c1[i]] + 1;
    d_b_dists[c1[i]] = true;
  }
}

__global__ void pearson_union(float* d_sum_xy, float* d_sum_x, float* d_sum_y, float* d_sum_x2, float* d_sum_y2, int* d_n, float* d_distances, bool* d_b_dists, int max_users){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < max_users){
    if(d_b_dists[i] == true){
      float num = ((d_n[i] * d_sum_xy[i]) - (d_sum_x[i] * d_sum_y[i]));
      float den1 = (d_n[i] * d_sum_y2[i] - (d_sum_y[i] * d_sum_y[i]));
      float den2 = (d_n[i] * d_sum_x2[i] - (d_sum_x[i] * d_sum_x[i]));
      if(den1 == 0 || den2 == 0 || d_n[i] < 2){
        d_distances[i] = 0;
      }
      else{
        d_distances[i] = num / sqrt(den1 * den2);
        // d_distances[i] = ((d_n[i] * d_sum_xy[i]) - (d_sum_x[i] * d_sum_y[i]));
      }

    }
    // d_distances[c1[i]] += abs(r1[i] - val);
  }
}

__global__ void manhatta_x_item(float* d_item_values, int* d_ind_items, int* d_item_col_ind, float* d_distances, bool* d_b_dists, float val, int n_users_x_movie, int id_movie){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users_x_movie){
    float* r1 = float_pointer(d_item_values, d_ind_items, id_movie);
    int* c1 = int_pointer(d_item_col_ind, d_ind_items, id_movie);
    d_distances[c1[i]] += abs(r1[i] - val);
    d_b_dists[c1[i]] = true;
  }
}

__global__ void euclidean_x_item(float* d_item_values, int* d_ind_items, int* d_item_col_ind, float* d_distances, bool* d_b_dists, float val, int n_users_x_movie, int id_movie){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_users_x_movie){
    float* r1 = float_pointer(d_item_values, d_ind_items, id_movie);
    int* c1 = int_pointer(d_item_col_ind, d_ind_items, id_movie);
    d_distances[c1[i]] += ((r1[i] - val) *(r1[i] - val));
    d_b_dists[c1[i]] = true;
  }
}

__global__ void desviacion_x_user(float* d_values, int* d_ind_users, int* d_col_ind, float* d_desviaciones, int* d_cardinalidad, bool* d_b_dists, float val, int n_movies_x_user, int id_user){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_movies_x_user){
    float* r1 = float_pointer(d_values, d_ind_users, id_user);
    int* c1 = int_pointer(d_col_ind, d_ind_users, id_user);
    d_desviaciones[c1[i]] += (val - r1[i]);
    d_cardinalidad[c1[i]]++;
    d_b_dists[c1[i]] = true;
  }
}

__global__ void coseno_x_user(floa11t* d_values, int* d_ind_users, int* d_col_ind, float* d_cosenos, float* d_averages, bool* d_b_dists, float val, float aver, int n_movies_x_user, int id_user){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i < n_movies_x_user){
    float* r1 = float_pointer(d_values, d_ind_users, id_user);
    int* c1 = int_pointer(d_col_ind, d_ind_users, id_user);
    d_desviaciones[c1[i]] += (val - r1[i]);
    d_cardinalidad[c1[i]]++;
    d_cosenos[c1[i]] =
    d_b_dists[c1[i]] = true;
  }
}

void distances_one2all_manhattan(float*& distances,  bool*& b_dists, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size,float* item_values, int* item_row_ind, int* item_col_ind, int* ind_items, int* item_row_size, float* d_item_values, int* d_item_row_ind, int* d_item_col_ind, int* d_ind_items, int* d_item_row_size, int n_users, int max_users, int id_user){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);

  b_dists = new bool[max_users];
  bool* d_b_dists = cuda_array<bool>(max_users);
  initialize_arr<bool>(b_dists, max_users, false);
  cuda_H2D<bool>(b_dists, d_b_dists, max_users);

  distances = new float[max_users];
  float* d_distances = cuda_array<float>(max_users);
  initialize_arr<float>(distances, max_users, 0);
  cuda_H2D<float>(distances, d_distances, max_users);

  float* ratings_user = float_pointer(values, ind_users, id_user);
  int* ids_movies = int_pointer(col_ind, ind_users, id_user);
  // for (size_t i = 0; i < row_size[id_user]; i++) {
  //   cout<<item_row_size[ids_movies[i]]<<endl;
  // }
  for (size_t i = 0; i < row_size[id_user]; i++) {
    // cout<<"pelicula: "<<ids_movies[i]<<endl;
    int n_users_x_movie = item_row_size[ids_movies[i]];
    // cout<< n_users_x_movie<<endl;

    dim3 grid =  dim3(ceil(n_users_x_movie / block_size), 1);

    manhatta_x_item<<<grid, block>>>(d_item_values, d_ind_items, d_item_col_ind, d_distances, d_b_dists, ratings_user[i], n_users_x_movie, ids_movies[i]);
    CHECK(cudaDeviceSynchronize());
  }
  cuda_D2H(d_distances, distances, max_users);
  cuda_D2H(d_b_dists, b_dists, max_users);

  // set<pair<float, int>, decltype(&compare_greater)> mapa(&compare_greater);
  // set<pair<float, int>, less<pair<float, int> > > mapa;
  // for (size_t i = 0; i < max_users; i++) {
  //   if(b_dists[i]){
  //     mapa.insert(make_pair(sqrt(distances[i]), i));
  //     // mapa[distances[i]] = dis
  //     // cout<<"i: "<<i<<" -> "<<distances[i]<<endl;
  //
  //   }
  // }
  //
  //
  // auto it = mapa.begin();
  // int i = 0;
  // for (; it != mapa.end() && (i < 10); it++) {
  //   i++;
  //   cout<<it->second<<" "<<it->first<<endl;
  // }

}


void desviaciones_one2all(float*& desviaciones, int*& cardinalidad,  bool*& b_dists, float* item_values, int* item_row_ind, int* item_col_ind, int* ind_items, int* item_row_size,float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, int n_movies, int max_movies, int id_movie){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);

  b_dists = new bool[max_movies];
  bool* d_b_dists = cuda_array<bool>(max_movies);
  initialize_arr<bool>(b_dists, max_movies, false);
  cuda_H2D<bool>(b_dists, d_b_dists, max_movies);

  desviaciones = new float[max_movies];
  float* d_desviaciones = cuda_array<float>(max_movies);
  initialize_arr<float>(desviaciones, max_movies, 0);
  cuda_H2D<float>(desviaciones, d_desviaciones, max_movies);

  cardinalidad = new int[max_movies];
  int* d_cardinalidad = cuda_array<int>(max_movies);
  initialize_arr<int>(cardinalidad, max_movies, 0);
  cuda_H2D<int>(cardinalidad, d_cardinalidad, max_movies);


  float* ratings_movie = float_pointer(item_values, ind_items, id_movie);
  int* ids_users = int_pointer(item_col_ind, ind_items, id_movie);
  // for (size_t i = 0; i < row_size[id_user]; i++) {
  //   cout<<item_row_size[ids_movies[i]]<<endl;
  // }
  for (size_t i = 0; i < item_row_size[id_movie]; i++) {
    // cout<<"pelicula: "<<ids_movies[i]<<endl;
    int n_movies_x_user = row_size[ids_users[i]];
    // cout<< n_users_x_movie<<endl;

    dim3 grid =  dim3(ceil(n_movies_x_user / block_size), 1);

    desviacion_x_user<<<grid, block>>>(d_values, d_ind_users, d_col_ind, d_desviaciones, d_cardinalidad, d_b_dists, ratings_movie[i], n_movies_x_user, ids_users[i]);
    CHECK(cudaDeviceSynchronize());
  }

  cout<<"aas"<<endl;
  cuda_D2H(d_desviaciones, desviaciones, max_movies);
  cuda_D2H(d_b_dists, b_dists, max_movies);
  cuda_D2H(d_cardinalidad, cardinalidad, max_movies);

  for (size_t i = 0; i < 10; i++) {
    if(b_dists[i])
      cout<<"i: "<< i<<" "<<desviaciones[i]<<" "<<cardinalidad[i]<<"  ->  "<<(desviaciones[i] / cardinalidad[i])<<endl;
      // distances[i] = sqrt(distances[i]);
  }

}


void coseno_ajustado_one2all(float*& cosenos,  bool*& b_dists, float* item_values, int* item_row_ind, int* item_col_ind, int* ind_items, int* item_row_size,float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, int n_movies, int max_movies, int id_movie, float* maxs, float* mins, float* averages){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);

  b_dists = new bool[max_movies];
  bool* d_b_dists = cuda_array<bool>(max_movies);
  initialize_arr<bool>(b_dists, max_movies, false);
  cuda_H2D<bool>(b_dists, d_b_dists, max_movies);

  cosenos = new float[max_movies];
  float* d_cosenos = cuda_array<float>(max_movies);
  initialize_arr<float>(cosenos, max_movies, 0);
  cuda_H2D<float>(cosenos, d_cosenos, max_movies);

  float* d_averages = cuda_array<float>(max_movies);
  cuda_H2D<float>(averages, d_averages, max_movies);



  float* ratings_movie = float_pointer(item_values, ind_items, id_movie);
  int* ids_users = int_pointer(item_col_ind, ind_items, id_movie);
  // for (size_t i = 0; i < row_size[id_user]; i++) {
  //   cout<<item_row_size[ids_movies[i]]<<endl;
  // }
  for (size_t i = 0; i < item_row_size[id_movie]; i++) {
    // cout<<"pelicula: "<<ids_movies[i]<<endl;
    int n_movies_x_user = row_size[ids_users[i]];
    // cout<< n_users_x_movie<<endl;

    dim3 grid =  dim3(ceil(n_movies_x_user / block_size), 1);

    desviacion_x_user<<<grid, block>>>(d_values, d_ind_users, d_col_ind, d_desviaciones, d_cardinalidad, d_b_dists, ratings_movie[i], n_movies_x_user, ids_users[i]);
    CHECK(cudaDeviceSynchronize());
  }

  cout<<"aas"<<endl;
  cuda_D2H(d_desviaciones, desviaciones, max_movies);
  cuda_D2H(d_b_dists, b_dists, max_movies);
  cuda_D2H(d_cardinalidad, cardinalidad, max_movies);

  for (size_t i = 0; i < 10; i++) {
    if(b_dists[i])
      cout<<"i: "<< i<<" "<<desviaciones[i]<<" "<<cardinalidad[i]<<"  ->  "<<(desviaciones[i] / cardinalidad[i])<<endl;
      // distances[i] = sqrt(distances[i]);
  }

}




void distances_one2all_euclidean(float*& distances,  bool*& b_dists, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size,float* item_values, int* item_row_ind, int* item_col_ind, int* ind_items, int* item_row_size, float* d_item_values, int* d_item_row_ind, int* d_item_col_ind, int* d_ind_items, int* d_item_row_size, int n_users, int max_users, int id_user){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);

  b_dists = new bool[max_users];
  bool* d_b_dists = cuda_array<bool>(max_users);
  initialize_arr<bool>(b_dists, max_users, false);
  cuda_H2D<bool>(b_dists, d_b_dists, max_users);

  distances = new float[max_users];
  float* d_distances = cuda_array<float>(max_users);
  initialize_arr<float>(distances, max_users, 0);
  cuda_H2D<float>(distances, d_distances, max_users);

  float* ratings_user = float_pointer(values, ind_users, id_user);
  int* ids_movies = int_pointer(col_ind, ind_users, id_user);
  // for (size_t i = 0; i < row_size[id_user]; i++) {
  //   cout<<item_row_size[ids_movies[i]]<<endl;
  // }
  for (size_t i = 0; i < row_size[id_user]; i++) {
    // cout<<"pelicula: "<<ids_movies[i]<<endl;
    int n_users_x_movie = item_row_size[ids_movies[i]];
    // cout<< n_users_x_movie<<endl;

    dim3 grid =  dim3(ceil(n_users_x_movie / block_size), 1);

    euclidean_x_item<<<grid, block>>>(d_item_values, d_ind_items, d_item_col_ind, d_distances, d_b_dists, ratings_user[i], n_users_x_movie, ids_movies[i]);
    CHECK(cudaDeviceSynchronize());
  }

  cout<<"aas"<<endl;
  cuda_D2H(d_distances, distances, max_users);
  cuda_D2H(d_b_dists, b_dists, max_users);
  for (size_t i = 0; i < max_users; i++) {
    if(b_dists[i])
      distances[i] = sqrt(distances[i]);
  }
  cout<<"aas"<<endl;

  // set<pair<float, int>, decltype(&compare_greater)> mapa(&compare_greater);
  // set<pair<float, int>, less<pair<float, int> > > mapa;
  // for (size_t i = 0; i < max_users; i++) {
  //   if(b_dists[i]){
  //     mapa.insert(make_pair(sqrt(distances[i]), i));
  //     // mapa[distances[i]] = dis
  //     cout<<"i: "<<i<<" -> "<<distances[i]<<endl;
  //
  //   }
  // }
  //
  //
  // auto it = mapa.begin();
  // int i = 0;
  // for (; it != mapa.end() && (i < 10); it++) {
  //   i++;
  //   cout<<it->second<<" "<<it->first<<endl;
  // }

}


void distances_one2all_pearson(float*& distances, bool*& b_dists, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size,float* item_values, int* item_row_ind, int* item_col_ind, int* ind_items, int* item_row_size, float* d_item_values, int* d_item_row_ind, int* d_item_col_ind, int* d_ind_items, int* d_item_row_size, int n_users, int max_users, int id_user){
  float block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);

  int* n = new int[max_users];
  initialize_arr<int>(n, max_users, 0);

  b_dists = new bool[max_users];
  bool* d_b_dists = cuda_array<bool>(max_users);
  initialize_arr<bool>(b_dists, max_users, false);
  cuda_H2D<bool>(b_dists, d_b_dists, max_users);

  distances = new float[max_users];
  float* d_distances = cuda_array<float>(max_users);
  initialize_arr<float>(distances, max_users, 0);
  cuda_H2D<float>(distances, d_distances, max_users);


  float* d_sum_xy = cuda_array<float>(max_users);
  cuda_H2D<float>(distances, d_sum_xy, max_users);
  float* d_sum_x = cuda_array<float>(max_users);
  cuda_H2D<float>(distances, d_sum_x, max_users);
  float* d_sum_y = cuda_array<float>(max_users);
  cuda_H2D<float>(distances, d_sum_y, max_users);
  float* d_sum_x2 = cuda_array<float>(max_users);
  cuda_H2D<float>(distances, d_sum_x2, max_users);
  float* d_sum_y2 = cuda_array<float>(max_users);
  cuda_H2D<float>(distances, d_sum_y2, max_users);
  int* d_n = cuda_array<int>(max_users);
  cuda_H2D<int>(n, d_n, max_users);


  float* ratings_user = float_pointer(values, ind_users, id_user);
  int* ids_movies = int_pointer(col_ind, ind_users, id_user);
  // for (size_t i = 0; i < row_size[id_user]; i++) {
  //   cout<<item_row_size[ids_movies[i]]<<endl;
  // }
  for (size_t i = 0; i < row_size[id_user]; i++) {
    // cout<<"pelicula: "<<ids_movies[i]<<endl;
    int n_users_x_movie = item_row_size[ids_movies[i]];
    // cout<< n_users_x_movie<<endl;

    dim3 grid =  dim3(ceil(n_users_x_movie / block_size), 1);

    pearson_x_item<<<grid, block>>>(d_item_values, d_ind_items, d_item_col_ind, d_sum_xy, d_sum_x, d_sum_y, d_sum_x2, d_sum_y2, d_n, d_b_dists, ratings_user[i], n_users_x_movie, ids_movies[i]);
    CHECK(cudaDeviceSynchronize());
  }
  dim3 grid =  dim3(ceil(max_users / block_size), 1);
  pearson_union<<<grid, block>>>(d_sum_xy, d_sum_x, d_sum_y, d_sum_x2, d_sum_y2, d_n, d_distances, d_b_dists, max_users);
  CHECK(cudaDeviceSynchronize());
  cuda_D2H<float>(d_distances, distances, max_users);
  cuda_D2H<bool>(d_b_dists, b_dists, max_users);

  // set<pair<float, int>, decltype(&compare_greater)> mapa(&compare_greater);
  // // set<pair<float, int>, less<pair<float, int> > > mapa;
  // for (size_t i = 0; i < max_users; i++) {
  //   if(b_dists[i]){
  //     mapa.insert(make_pair(distances[i], i));
  //     // mapa[distances[i]] = dis
  //     // cout<<"i: "<<i<<" -> "<<distances[i]<<endl;
  //
  //   }
  // }
  //
  //
  // auto it = mapa.begin();
  // int i = 0;
  // for (; it != mapa.end() && (i < 10); it++) {
  //   i++;
  //   cout<<it->second<<" "<<it->first<<endl;
  // }

}

void distances_one2all(float*& distances, bool*& b_dists, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size,float* item_values, int* item_row_ind, int* item_col_ind, int* ind_items, int* item_row_size, float* d_item_values, int* d_item_row_ind, int* d_item_col_ind, int* d_ind_items, int* d_item_row_size, int n_users, int max_users, int id_user, int measure){
  reloj r;
  r.start();
  switch (measure) {
    case EUCLIDEAN: distances_one2all_euclidean(distances, b_dists, values, row_ind, col_ind, ind_users, row_size, item_values, item_row_ind, item_col_ind, ind_items, item_row_size, d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, n_users, max_users, id_user);
      break;
    case PEARSON: distances_one2all_pearson(distances, b_dists, values, row_ind, col_ind, ind_users, row_size, item_values, item_row_ind, item_col_ind, ind_items, item_row_size, d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, n_users, max_users, id_user);
      break;
    case MANHATTAN: distances_one2all_manhattan(distances, b_dists, values, row_ind, col_ind, ind_users, row_size, item_values, item_row_ind, item_col_ind, ind_items, item_row_size, d_item_values, d_item_row_ind, d_item_col_ind, d_ind_items, d_item_row_size, n_users, max_users, id_user);
      break;
  }
  r.stop();
  cout<<"Tiempo de distancias de uno a todos: "<<r.time()<<"ms"<<endl;
}



#endif
