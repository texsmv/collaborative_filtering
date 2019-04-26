#ifndef K_MEANS_H
#define K_MEANS_H


#include "tipos.h"
#include <iostream>
#include <random>
#include "distances.h"


vector<t_ratings> convertir(vector<vector<t_rating>> v_means, vector<int> conts, int k){
  // centroides
  vector<t_ratings> n_means(k);

  int j = 0;
  auto it_cont = conts.b();
  for(auto it=v_means.b(); it != v_means.e(); it++){
    t_ratings ratings;
    int i = 0;
    for(auto ite = it->b(); ite != it->e(); ite++){
      if(*ite != 0){
        ratings.push_back(make_pair(i, (*ite) / *it_cont));

      }
    }
    n_means[j] = ratings;
    it_cont++;
    j++;
  }
  return n_means;

}

// void k_means(t_rating_vector& data, int k, int t, t_rating_matrix& matrix1, t_rating_matrix& matrix2){
//
//   // matrix = new t_rating_matrix*[k];
//   cout<<"Inicio"<<endl;
//   // inicializar centroides aleatorios de la data
//   vector<t_ratings> means(k);
//   cout<<"data size: "<<data.size()<<endl;
//
//   static std::random_device seed;
//   static std::mt19937 random_number_generator(seed());
//   std::uniform_int_distribution<size_t> indices(0, data.size() - 1);
//
//   // auto it = means.b();
//   for (int i = 0; i < means.size(); i++) {
//     // cout<<i<<endl;
//     int in = indices(random_number_generator);
//     cout<<"Indice: "<<in<<endl;
//     means[i] = *(data[indices(random_number_generator)].s);
//
//     for(auto ite = means[i].b(); ite != means[i].e();ite++){
//       cout<<ite->f<<" "<<ite->s<<endl;
//     }
//     // cout<<"Adsa"<<endl;
//     // it++;
//   }
//
//   // cout<<"dasd"<<endl;
//   // vector con las clases a la que pertenece cada dato
//   vector<int> data_cluster(data.size());
//
//   cout<<"Iniciando iteraciones"<<endl;
//   // for para el numero de iteraciones
//   for (size_t i = 0; i < t; i++) {
//     cout<<"Iteraciones : "<<i<<endl;
//     vector<int> means_cont(k);
//     vector<vector<t_rating> > n_means(k);
//     for (size_t j = 0; j < k; j++) {
//       n_means[j] = vector<t_rating>(n_peliculas);
//     }
//
//
//     // iterando los datos
//     int pos = 0;
//     for(auto it = data.b(); it != data.e(); it++){
//       t_similarity max_similarity = euclidean(&(means[0]), it->s);
//       int mean = 0;
//
//       // calculando similaridad a los centroides
//       for (size_t j = 1; j < means.size(); j++) {
//         t_similarity cur_similarity = euclidean(&(means[j]), it->s);
//         if(max_similarity < cur_similarity){
//           mean = j;
//           max_similarity = cur_similarity;
//         }
//       }
//       // guardando el cluster al que pertenece cada usuario
//       data_cluster[pos] = mean;
//       pos++;
//     }
//     auto it_data = data.b();
//     auto it_data_cluster = data_cluster.b();
//
//     while(it_data != data.e()) {
//       for (auto it = it_data->s->b(); it != it_data->s->e(); it++) {
//         n_means[*it_data_cluster][it->f] += it->s;
//       }
//       means_cont[*it_data_cluster]++;
//
//
//
//       it_data++;
//       it_data_cluster++;
//     }
//
//     //mal - considerar
//     means = convertir(n_means, means_cont, k);
//
//
//     for(auto v : data_cluster){
//       cout<<v<<" ";
//     }
//     cout<<endl;
//   }

//   // for(int a = 0; a < k; a++){
//   //   matrix[a] = new t_rating_matrix();
//   //   // cout<<"sas"<<endl;
//   // }
//   for(int a = 0; a < data_cluster.size(); a++){
//     // cout<<"da"<<endl;
//     if(data_cluster[a] == 0){
//       matrix1[data[a].f] = data[a].s;
//
//     }
//     else{
//       matrix2[data[a].f] = data[a].s;
//     }
//   }
//   cout<<matrix1.size()<<endl;
//   cout<<matrix2.size()<<endl;
//
// }
//

void k_means_r(t_rating_vector& data, int k, int t, t_rating_vector& matrix1, t_rating_vector& matrix2){

  // matrix = new t_rating_matrix*[k];
  cout<<"Inicio"<<endl;
  // inicializar centroides aleatorios de la data
  vector<t_ratings> means(k);
  cout<<"data size: "<<data.size()<<endl;
  // cout<<means.size()<<endl;

  static std::random_device seed;
  static std::mt19937 random_number_generator(seed());
  std::uniform_int_distribution<size_t> indices(0, data.size() - 1);

  // auto it = means.b();
  for (int i = 0; i < means.size(); i++) {
    // cout<<i<<endl;
    int in = indices(random_number_generator);
    cout<<"Indice: "<<in<<endl;
    means[i] = *(data[indices(random_number_generator)].s);

    // for(auto ite = means[i].b(); ite != means[i].e();ite++){
    //   cout<<ite->f<<" "<<ite->s<<endl;
    // }
    // cout<<"Adsa"<<endl;
    // it++;
  }

  // cout<<"dasd"<<endl;
  // vector con las clases a la que pertenece cada dato
  vector<int> data_cluster(data.size());
  vector<int> means_cont(k);

  cout<<"Iniciando iteraciones"<<endl;
  // for para el numero de iteraciones
  for (size_t i = 0; i < t; i++) {
    cout<<"Iteraciones : "<<i<<endl;
    means_cont = vector<int>(k);
    vector<vector<t_rating> > n_means(k);
    for (size_t j = 0; j < k; j++) {
      n_means[j] = vector<t_rating>(n_peliculas);
    }


    // iterando los datos
    int pos = 0;
    for(auto it = data.b(); it != data.e(); it++){
      t_similarity max_similarity = euclidean(&(means[0]), it->s);
      int mean = 0;

      // calculando similaridad a los centroides
      for (size_t j = 1; j < means.size(); j++) {
        t_similarity cur_similarity = euclidean(&(means[j]), it->s);
        if(max_similarity < cur_similarity){
          mean = j;
          max_similarity = cur_similarity;
        }
      }
      // guardando el cluster al que pertenece cada usuario
      data_cluster[pos] = mean;
      pos++;
    }
    auto it_data = data.b();
    auto it_data_cluster = data_cluster.b();

    while(it_data != data.e()) {
      for (auto it = it_data->s->b(); it != it_data->s->e(); it++) {
        n_means[*it_data_cluster][it->f] += it->s;
      }
      means_cont[*it_data_cluster]++;



      it_data++;
      it_data_cluster++;
    }

    //mal - considerar
    means = convertir(n_means, means_cont, k);


    for(auto v : data_cluster){
      cout<<v<<" ";
    }
    cout<<endl;
  }

  // for(int a = 0; a < k; a++){
  //   matrix[a] = new t_rating_matrix();
  //   // cout<<"sas"<<endl;
  // }
  matrix1 = t_rating_vector(means_cont[0]);
  matrix2 = t_rating_vector(means_cont[1]);
  int pos1 = 0;
  int pos2 = 0;
  for(int a = 0; a < data_cluster.size(); a++){
    // cout<<"da"<<endl;
    if(data_cluster[a] == 0){
      // cout<<0<<"   f "<<data[a].f<<endl;
      matrix1[pos1] = data[a];
      // cout<<matrix1[pos1].f<<endl;
      pos1++;
    }
    else{
      // cout<<0<<"   f "<<data[a].f<<endl;
      matrix2[pos2] = data[a];
      // cout<<matrix1[pos2].f<<endl;
      pos2++;
    }
  }
  // cout<<matrix1.size()<<endl;
  // cout<<matrix2.size()<<endl;

}


//
//
// void k_means_f(vector<int> data, int k, int t){
//   vector<float> means(k);
//
//   static std::random_device seed;
//   static std::mt19937 random_number_generator(seed());
//   std::uniform_int_distribution<size_t> indices(0, data.size() - 1);
//   list<int> data_cluster;
//
//   for (auto& cluster : means) {
//     cluster = data[indices(random_number_generator)];
//   }
//
//   for(auto v : means){
//     cout<<v<<endl;
//   }
//
//   for (size_t i = 0; i < t; i++) {
//     data_cluster.clear();
//     vector<float> n_means(k);
//     vector<int> means_cont(k);
//
//     for (auto it = data.begin(); it != data.end(); it++) {
//       int dist_min = abs(means[0] - *it);
//       int mean = 0;
//       for(int j = 1; j < means.size(); j++){
//         int dist_temp = abs(means[j] - *it);
//         if(dist_min > dist_temp){
//           mean = j;
//           dist_min = dist_temp;
//         }
//
//
//       }
//       data_cluster.push_back(mean);
//     }
//
//
//     auto it = data.begin();
//     auto ite = data_cluster.begin();
//     while (it != data.end()) {
//       n_means[*ite] += *it;
//       means_cont[*ite]++;
//       it++;
//       ite++;
//     }
//
//     for(int k = 0; k < n_means.size(); k++){
//       n_means[k] /=  means_cont[k];
//
//     }
//     means = n_means;
//
//   }
//   auto it = data_cluster.begin();
//   for(int i = 0; i < data.size(); i++){
//     cout<<data[i]<<" "<<*it;
//     it++;
//   }
//   cout<<endl;
//
//
//
//
// }


#endif
