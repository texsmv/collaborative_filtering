#ifndef KNN_H
#define KNN_H

typedef pair<float, int> par_fi;
typedef pair<int, float> par_if;

// void knn(float* distances, int* ids, float* dists, int pos, int k){
//   // ids = new int[k];
//   // dists = new float[k];
//   // par* p;
//   priority_queue<par, vector<par>, greater<par> > pq;
//   for (size_t i = 0; i < n_users; i++) {
//     pq.push(make_pair(distances[i], i));
//   }
//   for (size_t i = 0; i < k; i++) {
//     par p = pq.top();
//     ids[pos * k + i] = p.second;
//     dists[pos * k + i] = p.first;
//     pq.pop();
//   }
//
//
// }
//
//

void n_knns(float*& dists, int*& ids, int b, int n, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, int n_users, int k){
  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);

  dists = new float[n * k];
  ids = new int[n * k];

  float* distances = new float[n_users];
  float* d_distances = cuda_array<float>(n_users);
  for (size_t i = b; i < b + n; i++) {
    one2all_euclidean<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, i, n_users);
    CHECK(cudaDeviceSynchronize());
    cuda_D2H<float>(d_distances, distances, n_users);
    CHECK(cudaDeviceSynchronize());
    // knn(distances, ids, dists, i, k);
  }
}
/*

- se asume que distances tiene n_users distancias
- pos_users y dists_users no estan inicializados
- k es el numero de knns que se requiere
*/
void knn_euclidean(float* distances, int*& pos_users, float*& dists_users, int n_users, int k){
  pos_users = new int[n_users]; dists_users = new float[n_users];
  priority_queue<par_fi, vector<par_fi>, greater<par_fi> > pq;
  for (size_t i = 0; i < n_users; i++) {
    pq.push(make_pair(distances[i], i));
  }
  for (size_t i = 0; i < k; i++) {
    par_fi p = pq.top();
    pos_users[i] = p.second;
    dists_users[i] = p.first;
    pq.pop();
  }
}


void knn2(float* distances, int* ids, float* dists, int k, int n_users){
  // ids = new int[k];
  // dists = new float[k];
  // par* p;
  
  priority_queue<par, vector<par>, greater<par> > pq;
  for (size_t i = 0; i < n_users; i++) {
    pq.push(make_pair(distances[i], i));
  }
  for (size_t i = 0; i < k; i++) {
    par p = pq.top();
    ids[i] = p.second;
    dists[i] = p.first;
    pq.pop();
  }


}


#endif
