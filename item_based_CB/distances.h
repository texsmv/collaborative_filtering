#ifndef DISTANCES_H
#define DISTANCES_H

#include <limits>
#include <math.h>

#define CUDART_INF_F __int_as_float(0x7f800000)

__device__ float d_cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2, float* prom){
  float sum_Rui2 = 0;
  float sum_Ruj2 = 0;
  float sum_R = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;

  while (it1 < s1 || it2 < s2) {
    if(it1 < s1 && it2 < s2){
      if(col1[it1] == col2[it2]){
        sum_R += (r1[it1] - prom[col1[it1] - 1]) * (r2[it2] - prom[col2[it2] - 1]);
        sum_Rui2+= (r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1]);
        sum_Ruj2+= (r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1]);
        it1++; it2++;
        // n++;
      }else if(col1[it1] < col2[it2]){
         sum_Rui2+= ((r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1])); it1++;
      }else{
         sum_Ruj2+= ((r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1])); it2++;
      }
    }
    else{
      if(it1 < s1){
        sum_Rui2+= ((r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1])); it1++;
      }else{
        sum_Ruj2+= ((r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1])); it2++;
      }
    }
    n++;
  }
  // if(std::isnan( (sum_xy / (sqrt(sum_x2)*sqrt(sum_y2)) ))){
    // cout<<"n: "<<n<<" "<<" sum_xy: "<<sum_xy<<" sum_x2: "<<sum_x2<<" sum_y2: "<<sum_y2<<endl;
  // }

  return ((sum_Rui2 == 0) || (sum_Ruj2 == 0))? 0 : (sum_R / (sqrt(sum_Rui2)*sqrt(sum_Ruj2)));
}

__device__ float d_cosine2(float* r1, int* col1, int s1, float* r2, int* col2, int s2, float* prom){
  float sum_Rui2 = 0;
  float sum_Ruj2 = 0;
  float sum_R = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;

  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
      sum_R += (r1[it1] - prom[col1[it1] - 1]) * (r2[it2] - prom[col2[it2] - 1]);
      sum_Rui2+= (r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1]);
      sum_Ruj2+= (r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1]);
      it1++; it2++;
      // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  // if(std::isnan( (sum_xy / (sqrt(sum_x2)*sqrt(sum_y2)) ))){
    // cout<<"n: "<<n<<" "<<" sum_xy: "<<sum_xy<<" sum_x2: "<<sum_x2<<" sum_y2: "<<sum_y2<<endl;
  // }

  return ((sum_Rui2 == 0) || (sum_Ruj2 == 0))? 0 : (sum_R / (sqrt(sum_Rui2)*sqrt(sum_Ruj2)));
}

__host__ float cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2, float* prom){
  float sum_Rui2 = 0;
  float sum_Ruj2 = 0;
  float sum_R = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;

  // int n = 0;
  while (it1 < s1 || it2 < s2) {
    // cout<<"s: "<<s1<<" -- "<<s2<<endl;
    // cout<<"it: "<<it1<<"-"<<it2<<endl;
    // cout<<"col: "<<col1[it1]<<" "<<col2[it2]<<endl;
    // cout<<endl;
    if(it1 < s1 && it2 < s2){
      if(col1[it1] == col2[it2]){
        sum_R += (r1[it1] - prom[col1[it1] - 1]) * (r2[it2] - prom[col2[it2] - 1]);
        sum_Rui2+= (r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1]);
        sum_Ruj2+= (r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1]);
        it1++; it2++;
        // n++;
      }else if(col1[it1] < col2[it2]){
         sum_Rui2+= (r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1]); it1++;
      }else{
         sum_Ruj2+= (r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1]); it2++;
      }
    }
    else{
      if(it1 < s1){
        sum_Rui2+= (r1[it1] - prom[col1[it1] - 1]) * (r1[it1] - prom[col1[it1] - 1]); it1++;
      }else{
        sum_Ruj2+= (r2[it2] - prom[col2[it2] - 1]) * (r2[it2] - prom[col2[it2] - 1]); it2++;
      }
    }
    n++;
  }
  // if(std::isnan( (sum_xy / (sqrt(sum_x2)*sqrt(sum_y2)) ))){
    // cout<<"n: "<<n<<" "<<" sum_xy: "<<sum_xy<<" sum_x2: "<<sum_x2<<" sum_y2: "<<sum_y2<<endl;
  // }

  return ((sum_Rui2 == 0) || (sum_Ruj2 == 0))? 0 : (sum_R / (sqrt(sum_Rui2)*sqrt(sum_Ruj2)));
}



#endif
