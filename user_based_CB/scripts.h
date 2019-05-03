#ifndef SCRIPTS_H
#define SCRIPTS_H
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;

vector<string> split(const string& s, char delimiter){
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


class reloj{
public:
  void start(){c_start = clock();}
  void stop(){c_stop = clock();}
  double time(){return (double)(c_stop - c_start) * 1000.0 / CLOCKS_PER_SEC;}
  clock_t c_start, c_stop;
  double elapsed;
  reloj(){

  }
  ~reloj(){

  }

};


void n_of_users(string path, int& n_ratings, int& n_users, bool header){
  ifstream infile(path);
  string line;
  if(header) getline(infile, line);
  vector<string> tokens;

  int id_user, curr_id_user, users_counter, ratings_counter;

  ratings_counter = 0;  users_counter = 0;  id_user = -1;

  while (getline(infile, line)) {
    if(ratings_counter % 1000000 == 0){
      cout<<ratings_counter<<endl;
    }
    tokens = split(line, ',');
    curr_id_user = atoi(tokens[0].c_str());
    if(id_user < curr_id_user){
      users_counter++;
      id_user = curr_id_user;
    }
    ratings_counter++;
  }
  n_ratings = ratings_counter;
  n_users = users_counter;
  cout<<n_ratings<<" "<<n_users<<endl;
}

void read_ML_ratings(string path, int n_ratings, int n_users, bool header, float*& values, int*& row_ind, int*& col_ind, int*& ind_users, int*& row_size){
  values = new float[n_ratings];
  row_ind = new int[n_ratings];
  col_ind = new int[n_ratings];
  ind_users = new int[n_users];
  row_size = new int[n_users];


  ifstream infile(path);
  string line;
  if(header) getline(infile, line);
  vector<string> tokens;

  int id_user, curr_id_user, curr_id_item, users_counter, ratings_counter, n_r;
  float curr_rating;
  ratings_counter = 0;  users_counter = 0;  id_user = -1; n_r = 0;

  while (getline(infile, line)) {
    if(ratings_counter % 1000000 == 0)
      cout<<ratings_counter<<endl;
    tokens = split(line, ',');
    curr_id_user = atoi(tokens[0].c_str());
    curr_id_item = atoi(tokens[1].c_str());
    curr_rating = atof(tokens[2].c_str());
    if(id_user < curr_id_user){
      if(id_user != -1)
        row_size[users_counter - 1] = n_r;
      n_r = 0;
      ind_users[users_counter] = ratings_counter;
      id_user = curr_id_user;
      users_counter++;
    }

    values[ratings_counter] = curr_rating;
    row_ind[ratings_counter] = curr_id_user;
    col_ind[ratings_counter] = curr_id_item;
    n_r ++;
    ratings_counter++;
  }
  cout<<ratings_counter<<" - "<<users_counter<<endl;
  row_size[n_users - 1] = n_r;

}


void read_ML_movies(string path, map<int, string>& movie_names, bool header){
  ifstream infile(path);
  string line;
  if(header) getline(infile, line);
  vector<string> tokens;
  int movies_counter = 0;
  int id;
  while (getline(infile, line) ) {
    // if(movies_counter % 10000 == 0)
      // cout<<movies_counter<<"  "<<line<<endl;
    tokens = split(line, ',');
    id = atoi(tokens[0].c_str());
    movie_names[id] = tokens[1];
    // cout<<tokens[0]<<endl;
    movies_counter++;
  }
  cout<<"Numero de peliculas: "<<movies_counter<<endl;

}

#endif
