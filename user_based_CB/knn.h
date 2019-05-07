#ifndef KNN_H
#define KNN_H

#include <thrust/sort.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/copy.h>



typedef pair<float, int> par_fi;
typedef pair<int, float> par_if;


/*

- se asume que distances tiene n_users distancias
- pos_users y dists_users no estan inicializados
- k es el numero de knns que se requiere
*/
void knn_less(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[k]; dists_users = new float[k];
  priority_queue<par_fi, vector<par_fi>, greater<par_fi> > pq;
  for (size_t i = 0; i < n_users; i++) {
    if(i != user_pos)
      pq.push(make_pair(distances[i], i));
  }
  for (size_t i = 0; i < k; i++) {
    par_fi p = pq.top();
    pos_users[i] = p.second;
    dists_users[i] = p.first;
    pq.pop();
  }
}

void knn_greater(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[k]; dists_users = new float[k];
  priority_queue<par_fi, vector<par_fi>, less<par_fi> > pq;
  for (size_t i = 0; i < n_users; i++) {
    if(i != user_pos)
      pq.push(make_pair(distances[i], i));
  }
  for (size_t i = 0; i < k; i++) {
    par_fi p = pq.top();
    pos_users[i] = p.second;
    dists_users[i] = p.first;
    pq.pop();
  }
}

bool compare(const pair<float, int>&i, const pair<float, int>&j)
{
    return i.first < j.first;
}
bool compare2(const pair<float, int>&i, const pair<float, int>&j)
{
    return i.first > j.first;
}

bool compare_greater(const pair<float, int>&i, const pair<float, int>&j)
{
  if(i.first == j.first){
    return i.second < j.second;
  }
  return i.first > j.first;
}

void knn_greater_std(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[k]; dists_users = new float[k];
  vector< pair<float, int> >v;

  for (size_t i = 0; i < n_users; i++) {
    if(i != user_pos && distances[i] != 0)
      v.push_back(make_pair(distances[i], i));
  }
  sort(v.begin(),v.end(), greater<pair<float, int> >());
  for (size_t i = 0; i < k; i++) {
    pos_users[i] = v[i].second;
    dists_users[i] = v[i].first;
  }
}

void knn_less_std(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[k]; dists_users = new float[k];
  vector< pair<float, int> >v;

  for (size_t i = 0; i < n_users; i++) {
    if(i != user_pos)
      v.push_back(make_pair(distances[i], i));
  }
  sort(v.begin(),v.end());
  for (size_t i = 0; i < k; i++) {
    pos_users[i] = v[i].second;
    dists_users[i] = v[i].first;
  }
}

void knn_less_std2(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[k]; dists_users = new float[k];
  vector< pair<float, int> >v;

  for (size_t i = 0; i < n_users; i++) {
    if(i != user_pos)
      v.push_back(make_pair(distances[i], i));
  }
  sort(v.begin(),v.end());
  for (size_t i = 0; i < k; i++) {
    pos_users[i] = v[i].second;
    dists_users[i] = v[i].first;
  }
}

void knn_greater_std2(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[k]; dists_users = new float[k];
  vector< pair<float, int> >v;

  for (size_t i = 0; i < n_users; i++) {
    if(i != user_pos)
      v.push_back(make_pair(distances[i], i));
  }
  sort(v.begin(),v.end(), compare_greater);
  for (size_t i = 0; i < k; i++) {
    pos_users[i] = v[i].second;
    dists_users[i] = v[i].first;
  }
}




// Cuda knn, not used beacuse it has worse performance
void knn_less_cuda(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  // float* t_distances; int* t_pos_users;
  // t_distances = new float[n_users]; t_pos_users = new int[n_users];
  pos_users = new int[k]; dists_users = new float[k];

  thrust::host_vector<float> t_distances(n_users);
  thrust::host_vector<int> t_pos_users(n_users);
  thrust::device_vector<float> d_t_distances;
  thrust::device_vector<int> d_t_pos_users;


  for (size_t i = 0; i < n_users; i++) {
    t_distances[i] = (distances[i]);
    t_pos_users[i] = (i);
  }
  d_t_distances = t_distances;
  d_t_pos_users = t_pos_users;


  reloj r;
  r.start();
  thrust::sort_by_key(d_t_distances.begin(), d_t_distances.begin() + n_users, d_t_pos_users.begin());

  thrust::copy(d_t_distances.begin(), d_t_distances.end(), t_distances.begin());
  thrust::copy(d_t_pos_users.begin(), d_t_pos_users.end(), t_pos_users.begin());
  r.stop();
  cout<<"Cuda knns: "<<r.time()<<"ms"<<endl;

  int counter = 0;
  int i = 0;
  while (counter < k) {
    if(t_pos_users[i] != user_pos){
      dists_users[counter] = t_distances[i];
      pos_users[counter] = t_pos_users[i];
      counter++;
    }
    i++;
  }
}

void knn_greater_cuda(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  // float* t_distances; int* t_pos_users;
  // t_distances = new float[n_users]; t_pos_users = new int[n_users];
  pos_users = new int[k]; dists_users = new float[k];

  thrust::host_vector<float> t_distances(n_users);
  thrust::host_vector<int> t_pos_users(n_users);
  thrust::device_vector<float> d_t_distances;
  thrust::device_vector<int> d_t_pos_users;


  for (size_t i = 0; i < n_users; i++) {
    t_distances[i] = (distances[i]);
    t_pos_users[i] = (i);
  }
  d_t_distances = t_distances;
  d_t_pos_users = t_pos_users;


  reloj r;
  r.start();
  thrust::sort_by_key(d_t_distances.begin(), d_t_distances.begin() + n_users, d_t_pos_users.begin());

  thrust::copy(d_t_distances.begin(), d_t_distances.end(), t_distances.begin());
  thrust::copy(d_t_pos_users.begin(), d_t_pos_users.end(), t_pos_users.begin());
  r.stop();
  cout<<"Cuda knns: "<<r.time()<<"ms"<<endl;

  int counter = 0;
  int i = 0;
  while (counter < k) {
    if(t_pos_users[i] != user_pos){
      dists_users[counter] = t_distances[n_users - i - 1];
      pos_users[counter] = t_pos_users[n_users - i - 1];
      counter++;
    }
    i++;
  }
}


#endif
