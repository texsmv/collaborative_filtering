#include "tipos.h"
#include "read.h"

int main(int argc, char const *argv[]){
      t_rating_vector db(top);
      readBD("ratings.csv",db);
      // cout<<"sizedb: "<<db.size()<<endl;
      print(db);
  
  return 0;
}
