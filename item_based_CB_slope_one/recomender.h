#ifndef RECOMENDER_H
#define RECOMENDER_H


#include "cud_sparse_oper.h"
#include "distances.h"
#include <map>
#include <tuple>



float predecir(unsigned int* similarity_matrix, float* r1, int* col1, int s1, int pos_movie, int user_pos, int* pos_movies){
  float num = 0;
  float den = 0;
  cout<<"row size: "<<s1<<endl;
  int card;
  float desv;
  for (size_t it1 = 0; it1 < s1; it1++) {
    // float val = get_tm2(pos_movies[col1[it1]] ,pos_movie, similarity_matrix);
    unsigned int val = get_tm2(pos_movie ,pos_movies[col1[it1]], similarity_matrix);
    // cout<<val<<endl;
    card = val/10000;
    int desvI = val - (card * 10000);
    desv = desnormalizar(desvI / 1000.0);

    // cout<<card<<"  ->  "<<desv<<endl;

    num += ((r1[it1] + desv) * card);
    den += card;
  }
  // float val = desnormalize_data((SR /S), mins[user_pos], maxs[user_pos]);
  return (card == 0) ? 0 : (num / den);
}




#endif
