#ifndef RECOMENDER_H
#define RECOMENDER_H


#include "cud_sparse_oper.h"
#include "distances.h"
#include "scripts.h"
#include <map>
#include <tuple>



float predecir(float* similarity_matrix, float* maxs, float* mins, float* r1, int* col1, int s1, int pos_movie, int user_pos, int* pos_movies){
  float SR = 0;
  float S = 0;
  cout<<"row size: "<<s1<<endl;
  for (size_t it1 = 0; it1 < s1; it1++) {
    SR += normalize_data(r1[it1], mins[user_pos], maxs[user_pos]) * get_tm(pos_movie, pos_movies[col1[it1]], similarity_matrix);
    // S += abs(r1[it1]);
    S += abs(get_tm(pos_movie, pos_movies[col1[it1]], similarity_matrix));
  }
  float val = desnormalize_data((SR /S), mins[user_pos], maxs[user_pos]);
  return (S == 0) ? 0 : val;
}




#endif
