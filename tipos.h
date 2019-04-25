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
#define n_peliculas 27000


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


class Node;

class RecTree{
public:
  RecTree();
  ~RecTree();
private:
  Node* p_root;
  int max_depth;
  int max_node;
};




class Node{
public:
  Node();
  ~Node();
private:
  t_ratings centroid;
  int cur_depth;
  int max_elements;
  int cur_elements;
  Node* p_left;
  Node* p_right;

};



#endif
