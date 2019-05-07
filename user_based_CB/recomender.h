#ifndef RECOMENDER_H
#define RECOMENDER_H

#include "knn.h"
#include "distances.h"
#include <map>
#include <tuple>




void k_recomendaciones_propuesta(vector<int>& contador,vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k, map<int, string>& movies_names){
  cout<<endl;
  cout<<"K recomendaciones ordenadas: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_std2(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_std2(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;


  cout<<"Ids vecinos cercanos - distancias"<<endl;
  for (size_t i = 0; i < k; i++) {
    cout<<pos_users[i] + 1<<"  "<<dists_users[i]<<endl;
  }


  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);

  // float* vals_user1 = float_pointer(values, ind_users, 279674);
  // int* ids_movies_user1 = int_pointer(col_ind, ind_users, 279674);
  //
  // for (size_t i = 0; i < row_size[279674]; i++) {
  //   cout<<"otro "<<ids_movies_user1[i]<<" -> "<<vals_user1[i]<<endl;
  // }

  map<int, float> map_user;
  // cout<<"vistos: "<<endl;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<"user: "<< ids_movies_user[i]<<" -> "<<vals_user[i]<<endl;
  }



  map<string, pair<float, int>> ord_map;
  priority_queue<tuple<int, float, int>, vector<tuple<int, float, int> >, less<tuple<int, float, int>> > pq;
  map<int, pair<float, int> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      auto it = map_user.find(ids_movies[j]);
      if(it == map_user.end()){
        auto pelicula_it = movies.find(ids_movies[j]);
        if(pelicula_it == movies.end()){
          movies[ids_movies[j]] = make_pair(vals[j], 1);

        }
        else{
          pelicula_it->second.first += vals[j];
          pelicula_it->second.second += 1;
        }
      }
    }
  }

  for (auto it = movies.begin(); it != movies.end(); it++) {
    pq.push(make_tuple(it->second.second, it->second.first / it->second.second, it->first));
  }
  int counter = 0;
  while(counter < k && !pq.empty()) {
    tuple<int, float, int> pelicula = pq.top();
    if(get<1>(pelicula) >=3.5){
      ids_movies.push_back(get<2>(pelicula));
      movies_ratings.push_back(get<1>(pelicula));
      contador.push_back(get<0>(pelicula));
      counter++;
    }
    pq.pop();
    // cout<<"id: "<<it->first<<" -> "<<it->second.first / it->second.second<<endl;
  }
  cout<<"--------------------------------"<<endl;
  cout<<endl<<endl;

}



void k_recomendations_repetidos(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k){
  cout<<endl;
  cout<<"K recomendaciones: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_std(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_std(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;

  for (size_t i = 0; i < k; i++) {
    cout<<pos_users[i]<<" ->  "<<distances[pos_users[i]]<<" "<<dists_users[i]<<endl;
  }

  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);

  map<int, float> map_user;
  // cout<<"vistos: "<<endl;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<ids_movies_user[i]<<" -> "<<vals_user[n]<<endl;
  }

  map<int, tuple<float, int, bool> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    // cout<<pos_users[i]<<"numero de ratings"<<row_size[pos_users[i]]<<endl;
    // priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      auto it = map_user.find(ids_movies[j]);
      if(it == map_user.end()){
        auto pelicula_it = movies.find(ids_movies[j]);
        if(pelicula_it == movies.end()){
          if(vals[j] >= 4.5)
            movies[ids_movies[j]] = make_tuple(vals[j], 1, true);
          else
            movies[ids_movies[j]] = make_tuple(vals[j], 1, false);
        }
        else{
          get<0>(pelicula_it->second) += vals[j];
          get<1>(pelicula_it->second) += 1;
          if(vals[j] >= 4){
            get<2>(pelicula_it->second) = true;
          }

        }
      }

    }
  }

  for (auto it = movies.begin(); it != movies.end(); it++) {
    if(get<2>(it->second) == true){
      ids_movies.push_back(it->first);
      movies_ratings.push_back(get<0>(it->second) / get<1>(it->second));
    }
    // cout<<"id: "<<it->first<<" -> "<<it->second.first / it->second.second<<endl;
  }
  cout<<"--------------------------------"<<endl;
  cout<<endl<<endl;
}

void k_recomendations(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k){
  cout<<endl;
  cout<<"K recomendaciones: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_cuda(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_cuda(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;

  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);

  map<int, float> map_user;
  // cout<<"vistos: "<<endl;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<ids_movies_user[i]<<" -> "<<vals_user[i]<<endl;
  }

  map<int, pair<float, int> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    // cout<<pos_users[i]<<"numero de ratings"<<row_size[pos_users[i]]<<endl;
    priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      auto it = map_user.find(ids_movies[j]);
      if(it == map_user.end())
        pq.push(make_pair(vals[j], ids_movies[j]));
    }
    if(!pq.empty()){
      par_fi pelicula = pq.top();
      while (pelicula.first >= 4 && !pq.empty()) {
        auto pelicula_it = movies.find(pelicula.second);
        if(pelicula_it == movies.end()){
          movies[pelicula.second] = make_pair(pelicula.first, 1);
          // movies.push_back(make_pair(pelicula.second, pelicula.first));
        }
        else{
          pelicula_it->second.first += pelicula.first;
          pelicula_it->second.second += 1;
        }
        pq.pop();
        pelicula = pq.top();
      }
    }


  }

  for (auto it = movies.begin(); it != movies.end(); it++) {
    ids_movies.push_back(it->first);
    movies_ratings.push_back(it->second.first / it->second.second);
    // cout<<"id: "<<it->first<<" -> "<<it->second.first / it->second.second<<endl;
  }
  cout<<"--------------------------------"<<endl;
  cout<<endl<<endl;
}

void k_recomendations_1(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k){
  cout<<endl;
  cout<<"K recomendaciones: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_std(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_std(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;

  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);

  map<int, float> map_user;
  // cout<<"vistos: "<<endl;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<ids_movies_user[i]<<" -> "<<vals_user[i]<<endl;
  }

  map<int, pair<float, int> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    // cout<<pos_users[i]<<"numero de ratings"<<row_size[pos_users[i]]<<endl;
    priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      auto it = map_user.find(ids_movies[j]);
      if(it == map_user.end())
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
  cout<<"--------------------------------"<<endl;
  cout<<endl<<endl;
}

void k_ordered_recomendations_1(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k, map<int, string>& movies_names){
  cout<<endl;
  cout<<"K recomendaciones ordenadas: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_std2(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_std2(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;



  for (size_t i = 0; i < k; i++) {
    cout<<i<<"->"<<pos_users[i]<<"  "<<dists_users[i]<<endl;
  }


  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);

  float* vals_user1 = float_pointer(values, ind_users, 279674);
  int* ids_movies_user1 = int_pointer(col_ind, ind_users, 279674);

  for (size_t i = 0; i < row_size[279674]; i++) {
    cout<<"otro "<<ids_movies_user1[i]<<" -> "<<vals_user1[i]<<endl;
  }

  map<int, float> map_user;
  // cout<<"vistos: "<<endl;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<"user: "<< ids_movies_user[i]<<" -> "<<vals_user[i]<<endl;
  }


  map<string, pair<float, int>> ord_map;
  map<int, pair<float, int> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    // cout<<pos_users[i]<<"  numero de ratings: "<<row_size[pos_users[i]]<<endl;
    priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;
    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      auto it = map_user.find(ids_movies[j]);
      if(it == map_user.end())
        pq.push(make_pair(vals[j], ids_movies[j]));
    }
    if(!pq.empty()){
      par_fi pelicula = pq.top();
      while (pq.top().first == pelicula.first && !pq.empty()) {
        par_fi curr_peli = pq.top();
        ord_map[movies_names[curr_peli.second]] = curr_peli;
        pq.pop();
      }

      auto it = ord_map.begin();
      pelicula = (*it).second;
      while (true && pelicula.first >= 4) {
        auto pelicula_it = movies.find(pelicula.second);
        if(pelicula_it == movies.end()){
          movies[pelicula.second] = make_pair(pelicula.first, 1);
          break;
          // movies.push_back(make_pair(pelicula.second, pelicula.first));
        }
        else{
          pelicula_it->second.first += pelicula.first;
          pelicula_it->second.second += 1;
          it++;
          pelicula = (*it).second;
        }
      }


    }



  }

  for (auto it = movies.begin(); it != movies.end(); it++) {
    ids_movies.push_back(it->first);
    movies_ratings.push_back(it->second.first / it->second.second);
    // cout<<"id: "<<it->first<<" -> "<<it->second.first / it->second.second<<endl;
  }
  cout<<"--------------------------------"<<endl;
  cout<<endl<<endl;

}


void k_ordered_recomendations2(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int k, map<int, string>& movies_names){
  cout<<endl;
  cout<<"K recomendaciones ordenadas: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_cuda(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_cuda(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;

  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);

  map<int, float> map_user;
  // cout<<"vistos: "<<endl;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<ids_movies_user[i]<<" -> "<<vals_user[i]<<endl;
  }

  map<string, pair<float, int>> ord_map;
  map<int, pair<float, int> > movies;
  // float* t_ratings = new float[k]; int* t_ids = new int[k];
  for (size_t i = 0; i < k; i++) {
    // cout<<pos_users[i]<<"  numero de ratings: "<<row_size[pos_users[i]]<<endl;
    priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;

    float* vals = float_pointer(values, ind_users, pos_users[i]);
    int* ids_movies = int_pointer(col_ind, ind_users, pos_users[i]);
    for (size_t j = 0; j < row_size[pos_users[i]]; j++) {
      auto it = map_user.find(ids_movies[j]);
      if(it == map_user.end())
        pq.push(make_pair(vals[j], ids_movies[j]));
    }

    par_fi pelicula = pq.top();
    while (pq.top().first == pelicula.first) {
      par_fi curr_peli = pq.top();
      ord_map[movies_names[curr_peli.second]] = curr_peli;
      pq.pop();
    }

    pelicula = (*(ord_map.begin())).second;

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
  cout<<"--------------------------------"<<endl;
  cout<<endl<<endl;

}

float k_proyection(float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int n_ratings, int n_users, int measure, int pos_user, int id_movie, int k){

  float* vals_user = float_pointer(values, ind_users, pos_user);
  int* ids_movies_user = int_pointer(col_ind, ind_users, pos_user);
  map<int, float> map_user;
  for (size_t i = 0; i < row_size[pos_user]; i++) {
    map_user[ids_movies_user[i]] = vals_user[i];
    // cout<<ids_movies_user[i]<<" -> "<<vals_user[i]<<endl;
  }

  auto encontrado = map_user.find(id_movie);
  if(encontrado != map_user.end()){
    return -1;
  }
  cout<<endl;
  cout<<"Prediccion: "<<endl;
  cout<<"--------------------------------"<<endl;
  float* distances, *dists_users;
  int* pos_users;
  reloj r, r2;
  r.start();
  distances_one2all(distances, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, n_users, measure, pos_user);
  r.stop();
  cout<<"Calculo de distancias: "<<r.time()<<"ms"<<endl;

  r2.start();
  if((measure == PEARSON) || (measure == COSINE))
    knn_greater_cuda(distances, pos_users, dists_users, n_users, k, pos_user);
  else if(measure == EUCLIDEAN)
    knn_less_cuda(distances, pos_users, dists_users, n_users, k, pos_user);
  r2.stop();
  cout<<"Calculo de knns: "<<r2.time()<<"ms"<<endl;




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
        if(std::isnan(dists_users[i]))
          cout<<"NAN!: "<<"i: "<<i<<" "<< pos_users[i]<<" of size: "<<row_size[pos_users[i]]<<" id movie: "<<ids_movies[j]<<" distancia: "<<dists_users[i]<<endl;
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
  return recomendado;
}



#endif
