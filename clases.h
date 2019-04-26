#ifndef CLASES_H
#define CLASES_H

#include "k_means.h"

class Node{
public:
  Node(){};
  ~Node(){};

  void constructRecTree(t_rating_vector& data, int curr_depth);

private:
  t_ratings centroid;
  int cur_depth;
  int max_depth = 5;
  int max_elements = 1000;
  int cur_elements;
  Node* p_left =0;
  Node* p_right =0;
  t_rating_matrix* matrix;
  t_similarity_matrix* similarity_matrix;


};


void Node::constructRecTree(t_rating_vector& data, int curr_depth){

  if(data.size() < max_elements || curr_depth > max_depth){
    matrix = new t_rating_matrix();
    for(auto it = data.b(); it!= data.e(); it++){
      (*matrix)[it->f] = it->second;
    }
  }
  else{
    curr_depth++;
    t_rating_vector m1;
    t_rating_vector m2;
    k_means_r(data, 2, 1, m1, m2);

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
private:
  Node* p_root = 0;
  int max_depth = 5;
  int max_node = 5;
};


void RecTree::add_data(t_rating_vector& data){
  if(!p_root){
    p_root = new Node();
  }
  p_root->constructRecTree(data, 0);

}


#endif
