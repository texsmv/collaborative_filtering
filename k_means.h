#include "tipos.h"
#include <iostream>
#include <random>
#include "distances.h"


vector<t_ratings> convertir(vector<vector<t_rating>> v_means, vector<int> conts, int k){
  vector<t_ratings> n_means(k);

  int j = 0;
  auto it_cont = conts.b();
  for(auto it=v_means.b(); it != v_means.e(); it++){
    t_ratings ratings;
    int i = 0;
    for(auto ite = it->b(); ite != it->e(); ite++){
      ratings.push_back(make_pair(i, (*ite) / *it_cont));
    }
    n_means[j] = ratings;
    it_cont++;
    j++;
  }
  return n_means;

}

void k_means(t_rating_vector& data, int k, int t){
  // inicializar centroides aleatorios de la data
  vector<t_ratings> means(k);
  static std::random_device seed;
  static std::mt19937 random_number_generator(seed());
  std::uniform_int_distribution<size_t> indices(0, data.size() - 1);
  for (auto& cluster : means) {
    cluster = *(data[indices(random_number_generator)].s);
  }

  // vector con las clases a la que pertenece cada dato
  vector<int> data_cluster(data.size());


  // for para el numero de iteraciones
  for (size_t i = 0; i < t; i++) {
    vector<int> means_cont(k);
    vector<vector<t_rating> > n_means(k);\
    for (size_t j = 0; j < k; j++) {
      n_means[j] = vector<t_rating>(n_peliculas);
    }


    // iterando los datos
    int pos = 0;
    for(auto it = data.b(); it != data.e(); it++){
      t_similarity max_similarity = cosine(&(means[0]), it->s);
      int mean = 0;

      // calculando similaridad a los centroides
      for (size_t j = 1; j < means.size(); j++) {
        t_similarity cur_similarity = cosine(&(means[j]), it->s);
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
    means = convertir(n_means, means_cont, k);


  }




}

void k_means_f(vector<int> data, int k, int t){
  vector<float> means(k);

  static std::random_device seed;
  static std::mt19937 random_number_generator(seed());
  std::uniform_int_distribution<size_t> indices(0, data.size() - 1);
  list<int> data_cluster;

  for (auto& cluster : means) {
    cluster = data[indices(random_number_generator)];
  }

  for(auto v : means){
    cout<<v<<endl;
  }

  for (size_t i = 0; i < t; i++) {
    data_cluster.clear();
    vector<float> n_means(k);
    vector<int> means_cont(k);

    for (auto it = data.begin(); it != data.end(); it++) {
      int dist_min = abs(means[0] - *it);
      int mean = 0;
      for(int j = 1; j < means.size(); j++){
        int dist_temp = abs(means[j] - *it);
        if(dist_min > dist_temp){
          mean = j;
          dist_min = dist_temp;
        }


      }
      data_cluster.push_back(mean);
    }


    auto it = data.begin();
    auto ite = data_cluster.begin();
    while (it != data.end()) {
      n_means[*ite] += *it;
      means_cont[*ite]++;
      it++;
      ite++;
    }

    for(int k = 0; k < n_means.size(); k++){
      n_means[k] /=  means_cont[k];

    }
    means = n_means;

  }
  auto it = data_cluster.begin();

  for(int i = 0; i < data.size(); i++){
    cout<<data[i]<<" "<<*it<<endl;
    it++;
  }
}
