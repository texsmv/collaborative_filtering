#ifndef DISTANCES_H
#define DISTANCES_H

#include <limits>
#include <math.h>

#define CUDART_INF_F __int_as_float(0x7f800000)

__device__ float d_cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum_xy = 0;
  float sum_x2 = 0;
  float sum_y2 = 0;

  int it1 = 0;
  int it2 = 0;
  // int n = 0;
  while (it1 < s1 || it2 < s2) {
    if(it1 < s1 && it2 < s2){
      if(col1[it1] == col2[it2]){
        sum_xy += (r1[it1] * r2[it2]);
        sum_x2+= (r1[it1] * r1[it1]);
        sum_y2+= (r2[it2] * r2[it2]);
        it1++; it2++;
        // n++;
      }else if(col1[it1] < col2[it2]){
        sum_x2 += (r1[it1] * r1[it1]); it1++;
      }else{
        sum_y2+= (r2[it2] * r2[it2]); it2++;
      }
    }
    else{
      if(it1 < s1){
        it1++;  sum_x2 += (r1[it1] * r1[it1]);
      }else{
        it2++;  sum_y2+= (r2[it2] * r2[it2]);
      }
    }
  }
  return (sum_x2 == 0 || sum_y2 == 0)? 0 : (sum_xy / (sqrt(sum_x2)*sqrt(sum_y2)));
}

__host__ float cosine(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum_xy = 0;
  float sum_x2 = 0;
  float sum_y2 = 0;

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
        sum_xy += (r1[it1] * r2[it2]);
        sum_x2+= pow(r1[it1],2);
        sum_y2+= pow(r2[it2],2);
        it1++; it2++;
        // n++;
      }else if(col1[it1] < col2[it2]){
        it1++; sum_x2 += pow(r1[it1],2);
      }else{
        it2++; sum_y2+= pow(r2[it2],2);
      }
    }
    else{
      if(it1 < s1){
        it1++;  sum_x2 += pow(r1[it1],2);
      }else{
        it2++;  sum_y2+= pow(r2[it2],2);
      }
    }
    n++;
  }
  // if(std::isnan( (sum_xy / (sqrt(sum_x2)*sqrt(sum_y2)) ))){
    // cout<<"n: "<<n<<" "<<" sum_xy: "<<sum_xy<<" sum_x2: "<<sum_x2<<" sum_y2: "<<sum_y2<<endl;
  // }

  return ((sum_x2 == 0) || (sum_y2 == 0))? 0 : (sum_xy / (sqrt(sum_x2)*sqrt(sum_y2)));
}


__device__ float d_pearson(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum_xy = 0;
  float sum_x = 0;
  float sum_y = 0;
  float sum_y2 = 0;
  float sum_x2 = 0;
  int n = 0;

  int it1 = 0;
  int it2 = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        sum_xy += (r1[it1] * r2[it2]);
        sum_x += r1[it1];
        sum_y += r2[it2];
        sum_x2 += (r1[it1]) * (r1[it1]);
        sum_y2 += (r2[it2]) * (r2[it2]);
        it1++; it2++;
        n++;
    }else if(col1[it1] < col2[it2]){
      it1++;
    }else{
      it2++;
    }
  }


  if(n < 2){
    return 0;
  }



  float num =(sum_xy - (sum_x * sum_y / n));
  float den1 = (sqrt(sum_y2 - pow(sum_y,2)/n));
  float den2 = (sqrt(sum_x2 - pow(sum_x,2)/n));

  if( (n < 2) || (den1 == 0) ||  (den2 == 0) || num == 0){
    return 0;
  }
  // if ((num / (den1 * den2)) > 1){
  //   return 1;
  // }

  return  num / (den1 * den2);
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
    }else if(it1 < s1){
      it1++;
    }else{
      it2++;
    }
  }
  if(n == 0)
    return numeric_limits<float>::infinity();
  return sqrt(sum);
}


__host__ float pearson(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum_xy = 0;
  float sum_x = 0;
  float sum_y = 0;
  float sum_x2 = 0;
  float sum_y2 = 0;

  int n = 0;

  int it1 = 0;
  int it2 = 0;
  while (it1 < s1 && it2 < s2) {
    if(col1[it1] == col2[it2]){
        // cout<<"val1: "<<r1[it1]<<" val2: "<<r2[it2]<<endl;;
        sum_xy += (r1[it1] * r2[it2]);
        sum_x += r1[it1];
        sum_y += r2[it2];
        sum_x2 += (r1[it1] * r1[it1]);
        sum_y2 += (r2[it2] * r2[it2]);
        it1++; it2++;
        n++;
    }else if(it1 < s1){
      it1++;
    }else{
      it2++;
    }
  }

  // cout<<"n: "<<n<<" sum_xy: "<<sum_xy<<" sum_x: "<<sum_x<<" sum_y: "<<sum_y<<" sum_x2: "<<sum_x2<<" sum_y2: "<<sum_y2<<endl;
  // cout<<sum_y2<< "  ---  "<<pow(sum_y,2)/n<< " * "<<sum_x2 <<"  --  "<< pow(sum_x,2)/n<<endl;
  // cout<<sqrt(sum_y2 - pow(sum_y,2)/n)<<" - "<<sqrt(sum_x2 - pow(sum_x,2)/n) <<endl;
  // cout<<(sqrt(sum_y2 - pow(sum_y,2)/n) == 0)<<" - "<<(sqrt(sum_x2 - pow(sum_x,2)/n) ==0 )<<endl;
  if(n < 2){
    return 0;
  }

  float num =(sum_xy - (sum_x * sum_y / n));
  float den1 = (sqrt(sum_y2 - pow(sum_y,2)/n));
  float den2 = (sqrt(sum_x2 - pow(sum_x,2)/n));

  if( (n < 2) || (den1 == 0) ||  (den2 == 0) ){
    return 0;
  }

  return  num / (den1 * den2);
}




__device__ float d_euclidean(float* r1, int* col1, int s1, float* r2, int* col2, int s2){
  float sum = 0;

  int it1 = 0;
  int it2 = 0;
  int n = 0;
  while ((it1 < s1) && (it2 < s2)) {
    if(col1[it1] == col2[it2]){
      sum += ((r1[it1] - r2[it2]) * (r1[it1] - r2[it2]));
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





#endif
