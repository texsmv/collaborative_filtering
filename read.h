#ifndef READ_H
#define READ_H

#include <bits/stdc++.h>
#include <fstream>

using namespace std;

#define top 100

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
                              t_rating_matrix m1,m2;
                              k_means(db,2,10,m1,m2);
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



#endif