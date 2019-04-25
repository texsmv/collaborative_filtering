#ifndef TIPOS_H
#define TIPOS_H

#include <list>
#include <map>
#include <vector>
#include <tuple>

#define f first
#define s second

#define b begin
#define e end
#define n_peliculas 10


using namespace std;



typedef int t_id_user;
typedef int t_id_item;


typedef float t_rating;
typedef float t_distance;
typedef float t_similarity;
typedef list<pair<t_id_item, t_rating> > t_ratings;
typedef map<t_id_user, t_ratings*> t_rating_matrix;
typedef map<t_id_user, map<t_similarity, t_id_user>* > t_similarity_matrix;

typedef vector<pair<t_id_user, t_ratings*> > t_rating_vector;




class Node{
public:
  Node();
  ~Node();

  void constructRecTree(t_rating_vector data, int curr_depth);

private:
  t_ratings centroid;
  int cur_depth;
  int max_depth = 5;
  int max_elements;
  int cur_elements;
  Node* p_left;
  Node* p_right;


};


void Node::constructRecTree(t_rating_vector data, int curr_depth){

}


// class Node;

class RecTree{
public:
  RecTree();
  ~RecTree();

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
