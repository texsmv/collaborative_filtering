#include "sparse_operations.h"
#include <fstream>

#include <cstdlib>
#include <ctime>



int main(int argc, char const *argv[]) {



  float* values;
  int *row_ind, * col_ind;
  int * ind_users, *row_size;

  float* d_values;
  int *d_row_ind, * d_col_ind;
  int * d_ind_users, * d_row_size;

  float* distances = new float[n_users];
  float* d_distances;

  read_movieLens("ml-latest/ratings.csv", values, row_ind, col_ind, ind_users, row_size);

  d_values = cuda_array<float>(n_ratings);
  d_row_ind = cuda_array<int>(n_ratings);
  d_col_ind = cuda_array<int>(n_ratings);
  d_ind_users = cuda_array<int>(n_users);
  d_row_size = cuda_array<int>(n_users);

  d_distances = cuda_array<float>(n_users);

  cuda_H2D<float>(values, d_values, n_ratings);
  cuda_H2D<int>(row_ind, d_row_ind, n_ratings);
  cuda_H2D<int>(col_ind, d_col_ind, n_ratings);
  cuda_H2D<int>(ind_users, d_ind_users, n_users);
  cuda_H2D<int>(row_size, d_row_size, n_users);

  int block_size = 256;
  dim3 block =  dim3(block_size, 1, 1);
  dim3 grid =  dim3(ceil(n_users / block_size), 1);


  one2all<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, 3);

  CHECK(cudaDeviceSynchronize());
  cuda_D2H<float>(d_distances, distances, n_users);



  // d_row_ind = <int> cuda_array(n_ratings);

  // for (size_t i = 0; i < 10; i++) {
  //   cout<<values[i]<<" --> "<<row_ind[i]<<" "<<col_ind[i]<<endl;
  // }
  //
  // for (size_t i = 0; i < 4; i++) {
  //   cout<<"Ind: "<<ind_users[i]<<" stride: "<<row_size[i]<<endl;
  // }
  // cout<<"Ind: "<<ind_users[n_users - 1]<<" stride: "<<row_size[n_users - 1]<<endl;
  //
  //
  // float* r = float_pointer(values, ind_users, 1);
  //
  // for (size_t i = 0; i < row_size[1]; i++) {
  //   cout<<r[i]<<" - ";
  // }
  // cout<<endl;
  //


  // float* distances2 = new float[n_users];
  //
  // start4 = clock();
  //
  //
  // float v;
  // for (size_t i = 3; i < 4; i++) {
  //   float* r1 = float_pointer(values, ind_users, i);
  //   int* c1 = int_pointer(col_ind, ind_users, i);
  //   // if(i % 1000 == 0)
  //   // cout<<i<<endl;
  //   for (size_t j = 0; j < n_users; j++) {
  //     float* r2 = float_pointer(values, ind_users, j);
  //     int* c2 = int_pointer(col_ind, ind_users, j);
  //     v = euclidean(r1, c1, row_size[i], r2, c2, row_size[j]);
  //     distances2[j] = v;
  //     // if(v != 0)
  //     //   cout<<v<<endl;
  //
  //   }
  // }
  //
  // stop4 = clock();
  // elapsed4 = (double)(stop4 - start4) * 1000.0 / CLOCKS_PER_SEC;
  // cout<<"Tiempo proyeccion:  "<<elapsed4<<"ms"<<endl;
  //

  // for (size_t i = 0; i < 100; i++) {
  //   cout<<distances2[i]<< " - "<<distances[i]<<endl;
  // }

  float *dists;
  int *ids;
  clock_t start4 = clock();
  n_knns(dists, ids, 0, 1000, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size);
  clock_t stop4 = clock();
  double elapsed4 = (double)(stop4 - start4) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo proyeccion:  "<<elapsed4<<"ms"<<endl;
  // knn(distances, ids, dists, 0, n_users);
  // cout<<"Mas cercanos"<<endl;
  // for (size_t i = 0; i < n_knn * 4 ; i++) {
  //   if(dists[i] < numeric_limits<float>::infinity())
  //     cout<<ids[i]<<" -> "<<dists[i]<<endl;
  // }

  std::ofstream ofile("foobar.bin", std::ios::binary);
  ofile.write((char*)dists, sizeof(float) * n_knn * 4);

  std::ofstream ofile2("foobar2.bin", std::ios::binary);
  ofile2.write((char*)ids, sizeof(int) * n_knn * 4);



  // float * buffer = new float[n_knn * 4];
  // std::ifstream infile("foobar.bin", ios::binary | ios::in);
  // infile.read((char *)buffer, sizeof(float) * n_knn * 4);
  //
  // int * buffer2 = new int[n_knn * 4];
  // std::ifstream infile2("foobar2.bin", ios::binary | ios::in);
  // infile2.read((char *)buffer2, sizeof(int) * n_knn * 4);
  //
  // for (size_t i = 0; i < n_knn * 4 ; i++) {
  //   if(buffer[i] < numeric_limits<float>::infinity())
  //     cout<<buffer2[i]<<" -> "<<buffer[i]<<endl;
  // }
  //








  return 0;
}
