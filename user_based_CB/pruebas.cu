#include "scripts.h"
#include <iostream>
#include "structures.h"
#include "distances.h"

using namespace std;


int main(int argc, char const *argv[]) {
  int n_ratings, n_users;
  n_ratings = 27753444;
  n_users = 283228;
  // n_of_users("../databases/ml-latest/ratings.csv", n_ratings, n_users, true);
  // cout<<n_ratings<<" "<<n_users<<endl;

  float* values;
  int *row_ind, * col_ind;
  int * ind_users, *row_size;

  float* d_values;
  int *d_row_ind, * d_col_ind;
  int * d_ind_users, * d_row_size;

  read_ML("../databases/ml-latest/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);


  float* r1 = float_pointer(values, ind_users, 3);
  int* c1 = int_pointer(col_ind, ind_users, 3);

  float* r2 = float_pointer(values, ind_users, 1);
  int* c2 = int_pointer(col_ind, ind_users, 1);

  cout<<euclidean(r1, c1, row_size[3], r2, c2, row_size[1])<<endl;

  return 0;
}
