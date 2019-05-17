#ifndef RECOMENDER_H
#define RECOMENDER_H


#include "cud_sparse_oper.h"
#include "distances.h"
#include <map>
#include <tuple>



float predecir(float* similarity_matrix, float* r1, int* col1, int s1, int pos_movie, int user_pos, int* pos_movies){
  float num = 0;
  float den = 0;
  cout<<"row size: "<<s1<<endl;
  int card;
  float desv;
  for (size_t it1 = 0; it1 < s1; it1++) {
    float val = get_tm2(pos_movies[col1[it1]], pos_movie, similarity_matrix);
    if (val < 0) {
      card = -val;
      desv = desnormalizar(0);
    }
    else{
      float p_entera, p_flotante;
      p_flotante = modf(val, &p_entera);
      if(p_flotante == 0){
        card = p_entera - 1;
        desv = desnormalizar(1);
      }
      else{
        card = p_entera;
        desv = desnormalizar(p_flotante);
      }

    }

    num += ((r1[it1] + desv) * card);
    den += card;
  }
  // float val = desnormalize_data((SR /S), mins[user_pos], maxs[user_pos]);
  return (card == 0) ? 0 : (num / den);
}




#endif
