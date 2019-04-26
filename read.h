#ifndef READ_H
#define READ_H

#include <bits/stdc++.h>
#include <fstream>

using namespace std;

#define top 100000

#include "tipos.h"
#include "k_means.h"

vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


void print(t_rating_vector &m){
      ofstream w;
      w.open("w.txt");
      for(size_t i =0; i<m.size(); i++){
            w<< m[i].f<<"{";
            auto it = m[i].s->begin();
            while(it!=m[i].s->end()){
                  w<<"("<<it->f<<","<<it->s<<") ";
                  it++;
            }
            i++;
            w<<"}"<<endl;
      }
      w.close();

}

void readBD(string name, t_rating_vector &db){
      vector<string> tokens;
      int id,i,key;
      ifstream file;
      t_ratings* rt;
      string line;
      file.open(name);
      getline(file,line);
      key= 0; i=-1;
      if(file.is_open()){
            while(getline(file,line)){
                  tokens = split(line,',');
                  id = atoi(tokens[0].c_str());
                  if(id == key){
                        (*db[i].s).push_back(make_pair(atoi(tokens[1].c_str()),atof(tokens[2].c_str())));
                  }else{
                        i++;
                        if(i == top){ //llega al tope y realiza k_means
                              t_rating_vector m1,m2;
                              // print(db);
                              // break;
                              // k_means_r(db,2,2,m1,m2);
                              // limpia el vector y coloca los datos que obtuvo linea antes de realizar k_means para no perder los datos
                              db.clear();
                              key = id; i = 0;
                              db[i].f= id;
                              rt = new t_ratings();
                              (*rt).push_back(make_pair(atoi(tokens[1].c_str()),atof(tokens[2].c_str())));
                              db[i].second = rt;
                        }
                        else{ //halla nuevo id_user
                              key = id;
                              db[i].f= id;
                              rt = new t_ratings();
                              (*rt).push_back(make_pair(atoi(tokens[1].c_str()),atof(tokens[2].c_str())));
                              db[i].second = rt;
                        }
                  }

            }
            file.close();
      }else cout<<" BD closed"<<endl;
}


void read_file(t_rating_vector& db, string nombre){
  std::ifstream infile(nombre);
  std::string line;
  // std::istringstream iss(line);
  std::getline(infile, line);


  pair<t_id_user, t_ratings*> * record;
  int i = 0;
  int id_pel_max = 0;

  int id = -1;
  // int  cont = 0;
  while (std::getline(infile, line) && i < 1000000) {
    if(i % 1000000 == 0)
      cout<<i<<endl;
    std::istringstream iss(line);
    vector<string> tokens = split(line, ',');
    if(id != atoi(tokens[0].c_str())){
      if(id != -1)
        db.push_back(*record);

      record = new pair<t_id_user, t_ratings*> ();
      id = atoi(tokens[0].c_str());
      // cout<<id<<endl;
      (*record).f = atoi(tokens[0].c_str());
      (*record).s = new t_ratings();
      (*record).s->push_back(make_pair(atoi(tokens[1].c_str()), atof(tokens[2].c_str())));
      if(atoi(tokens[1].c_str()) > id_pel_max)
        id_pel_max = atoi(tokens[1].c_str());
    }
    else{
      (*record).s->push_back(make_pair(atoi(tokens[1].c_str()), atof(tokens[2].c_str())));
      if(atoi(tokens[1].c_str()) > id_pel_max)
        id_pel_max = atoi(tokens[1].c_str());
    }

    // cont++;
    i++;
  }
  cout<<"Total de ratings: "<<i<<endl;
  cout<<"Id maximo de peliculas: "<<id_pel_max<<endl;
  infile.close();
}



#endif
