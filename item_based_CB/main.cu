#include "scripts.h"

int main(int argc, char const *argv[]) {
  int n_ratings, n_users;
  int n_ratings_20, n_users_20, n_ratings_27, n_users_27;

  n_ratings_27 = 27753444;
  n_users_27 = 283228;

  n_ratings_20 = 20000263;
  n_users_20 = 138493;

  n_ratings = n_ratings_27;
  n_users = n_users_27;
  // n_ratings
  // n_of_users("../databases/ml-20m/ratings.csv", n_ratings, n_users, true);
  // cout<<n_ratings<<" "<<n_users<<endl;
  float* values;
  int *row_ind, * col_ind;
  int * ind_users, *row_size;
  float* maxs,*mins, *averages;

  float* item_values;
  int *item_row_ind, * item_col_ind;
  int * ind_items, *item_row_size;

  string path = "../databases/ml-latest/ratings.csv";

  // read_ML_ratings( path, n_ratings,  n_users, true  , values,row_ind, col_ind, ind_users, row_size,"27");

  // average_per_user(values,ind_users,row_size,maxs,mins,averages,n_users);

  read_ML_ratings_items(path, n_ratings, n_users, true,  item_values,  item_row_ind,  item_col_ind,  ind_items, item_row_size, "27");

  for (size_t i = 0; i < 40; i++) {
    cout<<item_values[i]<<endl;
    cout<<item_row_ind[i]<<endl;
    cout<<item_col_ind[i]<<endl;
    cout<<ind_items[i]<<endl;
    cout<<item_row_size[i]<<endl;
    cout<<endl;
  }


  return 0;
}
