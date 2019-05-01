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

  clock_t start2 = clock();
  // read_movieLens("ml-20millones/ratings.csv", values, row_ind, col_ind, ind_users, row_size);
  // read_movieLens("ml-latest/ratings.csv", values, row_ind, col_ind, ind_users, row_size);
  read_movieLens("ml-20m/ratings.csv", values, row_ind, col_ind, ind_users, row_size);
  clock_t stop2 = clock();
  double elapsed2 = (double)(stop2 - start2) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo de cargar bd:  "<<elapsed2<<"ms"<<endl;

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


  clock_t start3 = clock();
  one2all<<<grid, block>>>(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, d_distances, 3);
  clock_t stop3 = clock();
  double elapsed3 = (double)(stop3 - start3) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo uno a todos gpu:  "<<elapsed3<<"ms"<<endl;




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


  float* distances2 = new float[n_users];

  clock_t start4 = clock();


  float v;
  for (size_t i = 3; i < 4; i++) {
    float* r1 = float_pointer(values, ind_users, i);
    int* c1 = int_pointer(col_ind, ind_users, i);
    // if(i % 1000 == 0)
    // cout<<i<<endl;
    for (size_t j = 0; j < n_users; j++) {
      float* r2 = float_pointer(values, ind_users, j);
      int* c2 = int_pointer(col_ind, ind_users, j);
      v = euclidean(r1, c1, row_size[i], r2, c2, row_size[j]);
      distances2[j] = v;
      // if(v != 0)
      //   cout<<v<<endl;

    }
  }

  clock_t stop4 = clock();
  double elapsed4 = (double)(stop4 - start4) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo uno a todos cpu:  "<<elapsed4<<"ms"<<endl;


  // for (size_t i = 0; i < 100; i++) {
  //   cout<<distances2[i]<< " - "<<distances[i]<<endl;
  // }



  // float *dists;
  // int *ids;
  // clock_t start4 = clock();
  // n_knns(dists, ids, 0, 1, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size);
  // clock_t stop4 = clock();
  // double elapsed4 = (double)(stop4 - start4) * 1000.0 / CLOCKS_PER_SEC;
  // cout<<"Tiempo proyeccion:  "<<elapsed4<<"ms"<<endl;



  // knn(distances, ids, dists, 0, n_users);
  // cout<<"Mas cercanos"<<endl;
  // for (size_t i = 0; i < n_knn * 4 ; i++) {
  //   if(dists[i] < numeric_limits<float>::infinity())
  //     cout<<ids[i]<<" -> "<<dists[i]<<endl;
  // }

  // std::ofstream ofile("foobar.bin", std::ios::binary);
  // ofile.write((char*)dists, sizeof(float) * n_knn * 4);
  //
  // std::ofstream ofile2("foobar2.bin", std::ios::binary);
  // ofile2.write((char*)ids, sizeof(int) * n_knn * 4);


  start3 = clock();
  vector<int> recomendados = top_k_movies(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, 3, 100);
  for (size_t i = 0; i < recomendados.size(); i++) {
    // cout<<recomendados[i]<<" ";
  }
  cout<<endl;
  stop3 = clock();
  elapsed3 = (double)(stop3 - start3) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo top k:  "<<elapsed3<<"ms"<<endl;


  start3 = clock();
  float proyeccion = proyeccion_movies(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, 3, 10);
  stop3 = clock();
  elapsed3 = (double)(stop3 - start3) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Valor proyectado:  "<<proyeccion<<endl;
  cout<<"Tiempo proyeccion:  "<<elapsed3<<"ms"<<endl;



  int id;
  int id_pelicula;
  int k;
  while (true) {
    int opcion;
    cout<<"Id del usuario:"<<endl;
    // id =0;
    cin>>id;
    cout<<endl;
    // t_id_user id = db[0].f;
    // t_ratings r = *(db[0].s);
    // tree.get_sm(r, sm, rm);
    cout<<"1. para top K mas cercanos: "<<endl;
    cout<<"2. para prediccion dado k vecinos: "<<endl;

    cin>>opcion;
    // cout<<"Ingrese k : "<<endl;
    // cin>>k;

    if(opcion == 1){

      clock_t start4 = clock();
      vector<int> recomendados = top_k_movies(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, id, 100);
      clock_t stop4 = clock();
      double elapsed4 = (double)(stop4 - start4) * 1000.0 / CLOCKS_PER_SEC;
      cout<<"Tiempo :  "<<elapsed4<<"ms"<<endl;
      for (size_t i = 0; i < recomendados.size(); i++) {
        cout<<recomendados[i]<<" ";
      }
      cout<<endl;
    }
    else if(opcion == 2){
      cout<<endl;
      cout<<"Ingresar id pelicula"<<endl;
      cin>>id_pelicula;
      cout<<endl;
      float proyeccion = proyeccion_movies(d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, id, 10);
      cout<<"Valor proyectado:  "<<proyeccion<<endl;
    }
    else{
      break;
    }

    cout<<endl;
    cout<<endl;



    /* code */
  }





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
