#ifndef KNN_H
#define KNN_H

typedef pair<float, int> par_fi;
typedef pair<int, float> par_if;


/*

- se asume que distances tiene n_users distancias
- pos_users y dists_users no estan inicializados
- k es el numero de knns que se requiere
*/
void knn_less(float* distances, int*& pos_users, float*& dists_users, int n_users, int k, int user_pos){
  pos_users = new int[n_users]; dists_users = new float[n_users];
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
  pos_users = new int[n_users]; dists_users = new float[n_users];
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




#endif
