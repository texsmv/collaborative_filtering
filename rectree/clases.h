#ifndef CLASES_H
#define CLASES_H

#include "k_means.h"
#include "algoritmos.h"



// vector<t_id_user> knn(t_similarity_matrix &matrixS,t_id_user usr, int k){
//     vector<t_id_user> k_ng; //k_vecinos mas cercanos
//     auto e = matrixS.find(usr);
//     if(e == matrixS.e()){
//       cout<<"Not found!!!!!!!!!!! - GG"<<endl;
//     }
//     else{
//       cout<<"Found! - Size of map: "<<matrixS[usr]->size()<<endl;
//     }
//     auto itusr = matrixS[usr]->b();
//     while(itusr!= matrixS[usr]->e() && k>0 ){
//         k_ng.push_back(itusr->s);
//         itusr++;
//         k--;
//     }
//     return k_ng;
// }


class Node{
public:
  Node(){};
  ~Node(){};

  void constructRecTree(t_rating_vector& data, int curr_depth);
  void print_size();
  void get_sm(t_ratings&, t_similarity_matrix*& sm, t_rating_matrix*& rm);

private:
  t_ratings centroid;
  int cur_depth;
  int max_depth = 5;
  int max_elements = 100;
  int cur_elements;
  Node* p_left = 0;
  Node* p_right =0;
  t_rating_matrix* matrix;
  t_similarity_matrix* similarity_matrix;
  bool leaf =false;


};


void Node::get_sm(t_ratings& r, t_similarity_matrix*& sm, t_rating_matrix*& rm){
  if(leaf){
    sm = similarity_matrix;
    rm = matrix;
    cout<<"S: "<<similarity_matrix->size()<<endl;
    return;
  }
  t_similarity d_right = euclidean(&r, &p_right->centroid);
  t_similarity d_left = euclidean(&r, &p_left->centroid);
  if(d_right < d_left){
  // if(d_right < d_left){
    p_right->get_sm(r, sm, rm);
  }
  else{
    p_left->get_sm(r, sm, rm);
  }

}

void Node::print_size(){
  cout<<"Depth: "<<cur_depth<<"size: "<<cur_elements<<endl;
  if(p_left)
    p_left->print_size();
  if(p_right)
    p_right->print_size();

}


void Node::constructRecTree(t_rating_vector& data, int curr_depth){
  cur_elements = data.size();
  cur_depth = curr_depth;

  if(data.size() < max_elements || curr_depth > max_depth){
    leaf = true;
    matrix = new t_rating_matrix();
    for(auto it = data.b(); it!= data.e(); it++){
      (*matrix)[it->f] = it->second;
    }
    similarity_matrix = new t_similarity_matrix();
    calc_similarity_matrix(data, *similarity_matrix);
  }
  else{
    curr_depth++;
    t_rating_vector m1;
    t_rating_vector m2;


    // cout<<"m1 "<<m1.size()<<endl;
    // for(auto v : m1){
    //   cout<<v.f<<endl;
    // }

    // cout<<"m2 "<<m2.size()<<endl;
    // for(auto v : m2){
    //   cout<<v.f<<endl;
    // }

    // cout<<"here"<<endl;
    p_left = new Node();
    p_right = new Node();

    k_means_r(data, 2, 2, m1, m2, p_left->centroid, p_right->centroid);
    // cout<<"hhere"<<endl;
    p_left->constructRecTree(m1, curr_depth);
    // cout<<"hhhere"<<endl;
    p_right->constructRecTree(m2, curr_depth);
    // cout<<"hhhhere"<<endl;

  }
}



// class Node;

class RecTree{
public:
  RecTree(){};
  ~RecTree(){};

  void add_data(t_rating_vector&);
  void print_size();
  void get_sm(t_ratings& r, t_similarity_matrix*& sm, t_rating_matrix*& rm);
// private:
  Node* p_root = 0;
  int max_depth = 5;
  int max_node = 5;
};


void RecTree::get_sm(t_ratings& r, t_similarity_matrix*& sm, t_rating_matrix*& rm){

  p_root->get_sm(r, sm, rm);
}

void RecTree::add_data(t_rating_vector& data){
  if(!p_root){
    p_root = new Node();
  }
  p_root->constructRecTree(data, 0);

}

void RecTree::print_size(){
  p_root->print_size();
}


#endif
