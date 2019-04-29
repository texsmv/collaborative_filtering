#ifndef ALGORITMOS_H
#define ALGORITMOS_H
// #include <time.h>
#include <stdio.h>

#include "tipos.h"
#include <algorithm>


template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}


vector<t_id_user> knn(t_similarity_matrix &matrixS,t_id_user usr, int k){
    vector<t_id_user> k_ng; //k_vecinos mas cercanos
    auto itusr = matrixS[usr]->b();
    while(itusr!= matrixS[usr]->e() && k>0 ){
        k_ng.push_back(itusr->s);
        itusr++;
        k--;
    }
    return k_ng;
}

vector<pair<t_id_user,t_similarity> > knn_sim(t_similarity_matrix &matrixS,t_id_user usr, int k){
    vector<pair<t_id_user,t_similarity> > k_ng; //k_vecinos mas cercanos
    auto itusr = matrixS[usr]->b();
    while(itusr!= matrixS[usr]->e() && k>0 ){
        k_ng.push_back(make_pair(itusr->s,itusr->f));
        itusr++;
        k--;
    }
    return k_ng;
}
//
// vector<pair<t_id_user,t_similarity> > knn_sim2(t_similarity_matrix &matrixS,t_id_user usr, int k, t_id_item ){
//     vector<pair<t_id_user,t_similarity> > k_ng; //k_vecinos mas cercanos
//     auto itusr = matrixS[usr]->b();
//     while(itusr!= matrixS[usr]->e() && k>0 ){
//         k_ng.push_back(make_pair(itusr->s,itusr->f));
//         itusr++;
//         k--;
//     }
//     return k_ng;
// }

vector<t_ratings*> items_Knn(t_rating_matrix &rv, vector<t_id_user> &knn){
    vector<t_ratings*> list;
    t_ratings *t = new t_ratings();
    for(size_t i=0; i<knn.size();i++){
        t = rv[knn[i]];
        list.push_back(t);
    }
    return list;
}

void calc_similarity_matrix(t_rating_vector &matrixR, t_similarity_matrix &matrixS){
    t_id_user idusr,idng;
    t_similarity sum= 0;
    for(auto it=0; it < matrixR.size(); it++){
        idusr = matrixR[it].f;
        multimap<t_similarity, t_id_user> *sm = new multimap<t_similarity, t_id_user>();
        for(auto it1=0; it1 < matrixR.size(); it1++){
            if(it!=it1){
                idng = matrixR[it1].f;
                sum = euclidean(matrixR[it].s,matrixR[it1].s);
                // (*sm)[sum] = idng;
                (*sm).insert(make_pair(sum,idng));
                matrixS[idusr] = sm;
            }
        }
    }
    // cout<<"matriz de similitud: "<<matrixS.size()<<endl;
}

vector<t_id_item> top_k(vector<t_id_user> ids_usuarios,vector<t_ratings*> ratings, int k){
  vector<t_id_item> top;
  for(int i = 0; i < ids_usuarios.size(); i++){
    t_rating r_max = 0;
    t_id_item t_id = 0;
    for(auto it : *ratings[i]){
      if(it.second >= r_max){
        t_id = it.first;
        r_max = it.second;
      }
    }
    top.push_back(t_id);
  }
  return top;
}







bool find_item(t_ratings *a, t_id_item item,vector<t_rating> &rt){
    auto tmp = (*a).b();
    while(tmp != (*a).e()){
        if(tmp->f == item){
            rt.push_back(tmp->s);
            return true;
        }
        tmp++;
    }
    return false;
}

t_rating proyection(vector<pair<t_id_user,t_similarity> > &knn,t_rating_matrix &rv, t_id_item item){
    vector<pair<t_similarity,float> > porcent;
    vector<t_rating> r;
    float sum = 0;
    for(size_t i=0; i<knn.size();i++){
        if(find_item(rv[knn[i].f],item,r)){
            sum += knn[i].s;
            porcent.push_back(make_pair(knn[i].s,0));
        }
    }
    for(size_t i=0; i<porcent.size();i++){
        porcent[i].s = (porcent[i].f / sum);
    }
    t_rating proyect=0;
    for(size_t i=0; i<porcent.size();i++){
        proyect += r[i] * porcent[i].s;
    }
    return proyect;
}










#endif
