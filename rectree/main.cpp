#include "tipos.h"
#include "k_means.h"
#include "clases.h"
#include <cstdlib>
#include <ctime>
#include "read.h"
#include "algoritmos.h"


int randNum(int min, int max)
{
   return rand() % max + min;
}



int main(int argc, char const *argv[]) {
  // srand(time(0));
  // int count = 10000;
  // t_rating_vector ratings;
  // for (size_t i = 0; i < count; i++) {
  //   t_ratings* list1 = new t_ratings();
  //   int n = randNum(2, 100);
  //   for (size_t j = 0; j < n; j++) {
  //     pair<t_id_item, t_rating> a = make_pair(j, randNum(0, 5));
  //     list1->push_back(a);
  //   }
  //
  //   // cout<<randNum(0, 100)<<endl;
  //   // pair<t_id_item, t_rating> b = make_pair(1, 2);
  //   // pair<t_id_item, t_rating> c = make_pair(2, 1);
  //
  //   /* code */
  //   pair<t_id_user, t_ratings*> user1 = make_pair(i, list1);
  //
  //   ratings.push_back(user1);
  // }

  // t_rating_matrix matrix1;
  // t_rating_matrix matrix2;
  // t_rating_vector matrix1;
  // t_rating_vector matrix2;
  //
  // k_means_r(ratings, 2, 10, matrix1, matrix2);
  //
  // cout<<matrix1.size()<<endl;
  // cout<<matrix2.size()<<endl;

  // RecTree tree;
  // tree.add_data(ratings);

  // cout<<matrix[0]->size();
  // cout<<matrix[1]->size();
  // for(auto v = matrix[0]->b(); v!= matrix[0]->e();v++){
  //   cout<<v->f<<endl;
  // }

  // t_rating_vector db(top);
  // readBD("ml-latest/ratings.csv",db);
  // print(db);



  t_rating_vector db;
  t_similarity_matrix* sm;
  t_rating_matrix* rm;
  // read_file(db, "u.csv");
  clock_t start = clock();

  read_file(db, "ml-latest/ratings.csv");
  clock_t stop = clock();
  double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo de carga BD: "<<elapsed<<"ms"<<endl;

  RecTree tree;
  clock_t start2 = clock();
  tree.add_data(db);
  clock_t stop2 = clock();
  double elapsed2 = (double)(stop2 - start2) * 1000.0 / CLOCKS_PER_SEC;
  cout<<"Tiempo de construccion de la estructura: "<<elapsed2<<"ms"<<endl;
  // tree.print_size();


  t_id_user id;
  t_id_item id_pelicula;
  int k;
  while (true) {
    int opcion;
    cout<<"Id del usuario:"<<endl;
    id =0;
    // cin>>id;
    t_id_user id = db[0].f;
    t_ratings r = *(db[0].s);
    tree.get_sm(r, sm, rm);
    cout<<"1. para top K mas cercanos: "<<endl;
    cout<<"2. para prediccion dado k vecinos: "<<endl;
    vector<pair<t_id_user,t_similarity> > res_2;
    vector<t_id_user> res;
    vector<t_ratings*> res_r;
    // cin>>opcion;
    opcion = 1;
    cout<<"Ingrese k : "<<endl;
    // cin>>k;
    k =1000;

    if(opcion == 1){
      clock_t start3 = clock();


      res = knn(*sm, id, k);
      res_r = items_Knn(*rm, res);
      vector<t_id_item> topk = top_k(res, res_r, k);

      clock_t stop3 = clock();
      double elapsed3 = (double)(stop3 - start3) * 1000.0 / CLOCKS_PER_SEC;
      cout<<"Tiempo Top K:  "<<elapsed3<<"ms"<<endl;
      for(auto v : topk){
        // cout<<"Recomendados: "<< v<<endl;
      }
    }
    else if(opcion == 2){
      cout<<"Id pelicula a predecir"<<endl;
      // cin>>id_pelicula;
      id_pelicula = 10;
      clock_t start4 = clock();

      res_2 = knn_sim(*sm, id, k);
      t_rating a = proyection(res_2, *rm, id_pelicula);

      clock_t stop4 = clock();
      double elapsed4 = (double)(stop4 - start4) * 1000.0 / CLOCKS_PER_SEC;
      cout<<"Tiempo proyeccion:  "<<elapsed4<<"ms"<<endl;
      cout<<"Valor proyectado: "<<a<<endl;

    }
    else{
      break;
    }



    /* code */
  }





  // t_rating_vector m1, m2;
  // k_means_r(db, 2, 5, m1, m2);
  // cout<<m1.size()<<" "<<m2.size()<<endl;

  // cout<<"adding"<<endl;
  // cout<<"end adding"<<endl;
  //
  // cout<<endl;
  // cout<<endl;
  // cout<<endl;
  // cout<<sm->size()<<endl;

  // for(auto v : res){
  //   cout<<"Cercanos: "<< v<<endl;
  // }





  // pair<>


  // vector<int> v = {1,2,3,4,5,6,7,8,9,10};
  // k_means_f(v, 3, 1000);

  return 0;
}
