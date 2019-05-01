#ifndef DISTANCES_H
#define DISTANCES_H

#include <limits>

#define CUDART_INF_F __int_as_float(0x7f800000)

__device__ float d_cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float lxl = 0;
  float lyl = 0;

  int it1 = 0;
  int it2 = 0;
  // int n = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        lxl+= pow(r1[it1],2);
        lyl+= pow(r2[it2],2);
        it1++; it2++;
         // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  return (lxl == 0 || lyl == 0)? CUDART_INF_F : (xy / (sqrt(lxl)*sqrt(lyl)));
}

__host__ float cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float lxl = 0;
  float lyl = 0;

  int it1 = 0;
  int it2 = 0;
  // int n = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        lxl+= pow(r1[it1],2);
        lyl+= pow(r2[it2],2);
        it1++; it2++;
         // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  return (lxl == 0 || lyl == 0)? numeric_limits<float>::infinity() : (xy / (sqrt(lxl)*sqrt(lyl)));
}


__device__ float d_pearson(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float x = 0;
  float y = 0;
  float y_2 = 0;
  float x_2 = 0;
  int n = 0;

  int it1 = 0;
  int it2 = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        x += r1[it1];
        y += r2[it2];
        x_2 += (r1[it1]) * (r1[it1]);
        y_2 += (r2[it2]) * (r2[it2]);
        it1++; it2++;
        n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  float n1 = xy - (( x * y )/n);
  float d1 = sqrt(x_2 -((x*x)/n));
  float d2 = sqrt(y_2 -((y*y)/n));
  return (n == 0) ? 0: n1 / (d1 * d2);
}

__host__ float pearson(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float xy = 0;
  float x = 0;
  float y = 0;
  float y_2 = 0;
  float x_2 = 0;
  int n = 0;

  int it1 = 0;
  int it2 = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        xy += (r1[it1] * r2[it2]);
        x += r1[it1];
        y += r2[it2];
        x_2 += pow(r1[it1],2);
        y_2 += pow(r2[it2],2);
        it1++; it2++;
        n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  float n1 = xy - (( x * y )/n);
  float d1 = sqrt(x_2 -(pow(x,2)/n));
  float d2 = sqrt(y_2 -(pow(y,2)/n));
  return (n == 0) ? 0 : n1 / (d1 * d2);
}




__device__ float d_euclidean(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
      sum += (r1[it1] - r2[it2]) * (r1[it1] - r2[it2]);
      it1++; it2++; n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  if(n == 0)
    return CUDART_INF_F;
  return sqrt(sum);
}

__host__ float euclidean(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
      sum += (r1[it1] - r2[it2]) * (r1[it1] - r2[it2]);
      it1++; it2++; n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }
  if(n == 0)
    return numeric_limits<float>::infinity();
  return sqrt(sum);
}






#endif
