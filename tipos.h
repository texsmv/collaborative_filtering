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
#define n_peliculas 200000


using namespace std;



typedef int t_id_user;
typedef int t_id_item;


typedef float t_rating;
typedef float t_distance;
typedef float t_similarity;
typedef list<pair<t_id_item, t_rating> > t_ratings;
typedef map<t_id_user, t_ratings*> t_rating_matrix;
typedef map<t_id_user, multimap<t_similarity, t_id_user>* > t_similarity_matrix;

typedef vector<pair<t_id_user, t_ratings*> > t_rating_vector;






#endif
