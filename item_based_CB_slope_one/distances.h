#ifndef DISTANCES_H
#define DISTANCES_H

#include <limits>
#include <math.h>

#define CUDART_INF_F __int_as_float(0x7f800000)

__host__ __device__ float normalizar(float x){
  return (x - (-5.0)) / (5.0 - (-5.0));
}

__host__ __device__ float desnormalizar(float x){
  return (x * (5.0 - (-5.0)) + (-5.0));
}

__device__ unsigned int d_desviacion(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;
  int cardinalidad = 0;
  int it1, it2;
  it1 = 0; it2 = 0;
  while (it1 < s1 && it2 < s2){
    if(col1[it1] == col2[it2]){
      sum += (r1[it1] - r2[it2]);
      cardinalidad++;
      it1++; it2++;
      // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }

  if(cardinalidad == 0){
    return 0;
  }

  int desv = int(normalizar(sum / (float)cardinalidad) * 1000) ;
  // cout<<(desv + (cardinalidad * 10000))<<endl;
  return (desv + (cardinalidad * 10000));

  // if(cardinalidad == 0){
  //   return 0;
  // }
  //
  // float desv = normalizar(sum / cardinalidad);
  //
  // if(desv == 0){
  //   return -cardinalidad;
  // }
  // return (desv + cardinalidad);
}

__host__ unsigned int desviacion(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;
  int cardinalidad = 0;
  int it1, it2;
  it1 = 0; it2 = 0;
  while (it1 < s1 && it2 < s2){
    if(col1[it1] == col2[it2]){
      sum += (r1[it1] - r2[it2]);
      cardinalidad++;
      it1++; it2++;
      // n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }


  // return (sum / cardinalidad);
  if(cardinalidad == 0){
    return 0;
  }

  int desv = int(normalizar(sum / (float)cardinalidad) * 1000) ;
  // cout<<(desv + (cardinalidad * 10000))<<endl;
  return (desv + (cardinalidad * 10000));
  // cout<<(sum / (float)cardinalidad)<<" "<<(desv )<<" "<<((int(desv * 1000) ) + cardinalidad * 10000)<<" "<<cardinalidad<<endl;

  // if(desv == 0){
  //   return -cardinalidad;
  // }
  // return (desv + cardinalidad);
}


#endif
