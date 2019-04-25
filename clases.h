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
  int max_elements = 10;
  int cur_elements;
  Node* p_left;
  Node* p_right;


};


void Node::constructRecTree(t_rating_vector& data, int curr_depth){
  if(data.size() < max_elements || curr_depth > max_depth){

  }
  else{
    cur_depth++;
    t_rating_matrix m1;
    t_rating_matrix m2;
    k_means(data, 2, 10, m1, m2);

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
  int max_depth;
  int max_node;
};


void RecTree::add_data(t_rating_vector& data){
  if(!p_root){
    p_root = new Node();
  }
  p_root->constructRecTree(data, 0);

}


#endif
