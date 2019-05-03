#ifndef RECOMENDER_H
#define RECOMENDER_H

#include "knn.h"
#include "distances.h"
#include <map>
#include <tuple>

void k_recomendations(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k){
  float* distances, *dists_users;
  int* pos_users;
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  knn_pearson(distances, pos_users, dists_users, n_users, k, pos_user);

  for (size_t i = 0; i < k; i++) {
    cout<<"pos: "<< pos_users[i]<<" - >"<<"distancia: "<<dists_users[i]<<endl;
  }

  map<int, pair<float, int> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    cout<<pos_users[i]<<"numero de ratings"<<row_size[pos_users[i]]<<endl;
    priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      pq.push(make_pair(vals[j], ids_movies[j]));
    }
    par_fi pelicula = pq.top();
    auto pelicula_it = movies.find(pelicula.second);
    if(pelicula_it == movies.end()){
      movies[pelicula.second] = make_pair(pelicula.first, 1);
      // movies.push_back(make_pair(pelicula.second, pelicula.first));
    }
    else{
      pelicula_it->second.first += pelicula.first;
      pelicula_it->second.second += 1;
    }
  }

  for (auto it = movies.begin(); it != movies.end(); it++) {
    ids_movies.push_back(it->first);
    movies_ratings.push_back(it->second.first / it->second.second);
    // cout<<"id: "<<it->first<<" -> "<<it->second.first / it->second.second<<endl;
  }

}

float k_proyection(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int id_movie, int k){
  float* distances, *dists_users;
  int* pos_users;
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  knn_pearson(distances, pos_users, dists_users, n_users, k, pos_user);

  vector<int> ids_s;
  vector<float> distances_s;
  vector<float> valoraciones_s;

  float sum = 0;
  int n = 0;

  // for (size_t i = 0; i < k; i++) {
  //   cout<<"pos: "<< pos_users[i]<<" - >"<<"distancia: "<<dists_users[i]<<endl;
  // }

  for (size_t i = 0; i < k; i++) {
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      // cout<<"It: "<<j<<endl;
      if(ids_movies[j] == id_movie){
        if(dists_users[i] != 0){
          cout<<"aqui!!!"<<endl;
        }
        if(std::isnan(dists_users[i]))
          cout<<"i: "<<i<<" "<< pos_users[i]<<" of size: "<<row_size[pos_users[i]]<<" id movie: "<<ids_movies[j]<<" distancia: "<<dists_users[i]<<endl;
        // cout<<distances[i]<<endl;
        distances_s.push_back(dists_users[i]);
        valoraciones_s.push_back(vals[j]);
        ids_s.push_back(pos_users[i]);
        sum += dists_users[i]; n++;
        break;
      }
    }

  }
  if(sum == 0){
    return 0;
  }

  float recomendado = 0;
  for (size_t i = 0; i < ids_s.size(); i++) {
    // cout<<distances_s[i]<<"  "<<valoraciones_s[i]<<endl;
    recomendado += (distances_s[i] / sum) * valoraciones_s[i];
  }
  cout<<"n: "<<n<<" "<<"Suma: "<<sum<<endl;
  cout<<recomendado<<endl;
}



float proyeccion_movies(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int id_user, int id_pelicula, int n_users){
  cout<<"k movies"<<endl;
  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);
  vector<int> k_movies;
  int* ids = new int[n_knn];
  float* dists = new float[n_knn];

  float* distances = new float[n_users];
  float* d_distances = cuda_array<float>(n_users);
  one2all_euclidean<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, id_user, n_users);
  CHECK(cudaDeviceSynchronize());
  cuda_D2H<float>(d_distances, distances, n_users);
  CHECK(cudaDeviceSynchronize());

  knn2(distances, ids, dists, n_knn, n_users);

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



#endif
