#include <TGUI/TGUI.hpp>
#include "scripts.h"
#include <iostream>
#include "structures.h"
#include "distances.h"
#include "cud_sparse_oper.h"
#include "recomender.h"

using namespace std;

void recomendar(vector<int>& ids_movies, vector<float>& movies_ratings, float* d_values, int* d_row_ind, int* d_col_ind, int* d_ind_users, int* d_row_size, float* values, int* row_ind, int* col_ind, int* ind_users, int* row_size, int& n_ratings, int& n_users, int measure, int pos_user, int k, map<int, string>& movies_names){
  // cout<<"recomendar: "<<a<<endl;
  cout<<n_ratings<<" "<<n_users<<endl;
  reloj r;
  r.start();
  cout<<"init"<<endl;
  k_ordered_recomendations(ids_movies, movies_ratings, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, n_ratings, n_users, COSINE, 4, 10, movies_names);
  cout<<"end"<<endl;
  r.stop();
  cout<<r.time()<<"ms"<<endl;
  for (size_t i = 0; i < ids_movies.size(); i++) {
    cout<<movies_names[ids_movies[i]]<<" "<<movies_ratings[i]<<endl;
  }
}




void load_dataset(bool& load, string option,int& n_ratings, int& n_users, float*& values, int*& row_ind, int*& col_ind, int*& ind_users, int*& row_size, float*& d_values, int*& d_row_ind, int*& d_col_ind, int*& d_ind_users, int*& d_row_size, map<int, string>& movies_names){
  cout<<option<<endl;
  if(load && option != ""){
    cout<<"In load function"<<endl;
    load = false;
    // cout<<"Opcion: "<<load<<endl;
    if(option == "Movie Lens 27M"){
      // cout<<option<<endl;
      n_ratings = 27753444;
      n_users = 283228;
      read_ML_movies("../databases/ml-latest/movies.csv", movies_names, true);
      read_ML_ratings("../databases/ml-latest/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);
    }
    else if(option == "Movie Lens 20M"){
      n_ratings = 20000263;
      n_users = 138493;
      read_ML_movies("../databases/ml-20m/movies.csv", movies_names, true);
      read_ML_ratings("../databases/ml-20m/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);
    }
    if(option != ""){
      d_values = cuda_array<float>(n_ratings);
      d_row_ind = cuda_array<int>(n_ratings);
      d_col_ind = cuda_array<int>(n_ratings);
      d_ind_users = cuda_array<int>(n_users);
      d_row_size = cuda_array<int>(n_users);

      cuda_H2D<float>(values, d_values, n_ratings);
      cuda_H2D<int>(row_ind, d_row_ind, n_ratings);
      cuda_H2D<int>(col_ind, d_col_ind, n_ratings);
      cuda_H2D<int>(ind_users, d_ind_users, n_users);
      cuda_H2D<int>(row_size, d_row_size, n_users);



    }

  }


  // load = true;

}



int main(){


    int n_ratings, n_users;
    n_ratings = 0;
    n_users = 0;

    float* values;
    int *row_ind, * col_ind;
    int * ind_users, *row_size;

    float* d_values;
    int *d_row_ind, * d_col_ind;
    int * d_ind_users, * d_row_size;

    map<int, string> movies_names;


    bool load_data = true;
    vector<int> ids_movies;
    vector<float> movies_ratings;


    // User interface

    sf::RenderWindow window{{800, 600}, "Window"};
    tgui::Gui gui{window}; // Create the gui and attach it to the window


    unsigned int b_load, b_recomend;
    tgui::ListBox::Ptr list_dbs = tgui::ListBox::create();
    tgui::Button::Ptr load_button = tgui::Button::create();
    tgui::Button::Ptr recommend_button = tgui::Button::create();
    tgui::Button::Ptr proyect_button = tgui::Button::create();
    tgui::EditBox::Ptr editBox_id_user = tgui::EditBox::create();

    load_button->setPosition(50, 50);
    recommend_button->setPosition(300, 500);
    proyect_button->setPosition(300, 400);
    list_dbs->setPosition(300, 50);



    list_dbs->addItem("Movie Lens 27M");
    list_dbs->addItem("Movie Lens 20M");

    gui.add(load_button, "LoadButton");
    gui.add(recommend_button, "RecommendButton");
    gui.add(proyect_button, "ProyectButton");
    gui.add(editBox_id_user, "idUserInput");
    gui.add(list_dbs, "Mylist");


    b_load = load_button->connect("pressed", load_dataset, std::ref(load_data), list_dbs->getSelectedItem(),
    std::ref(n_ratings), std::ref(n_users), std::ref(values), std::ref(row_ind),
    std::ref(col_ind), std::ref(ind_users), std::ref(row_size), std::ref(d_values),
    std::ref(d_row_ind), std::ref(d_col_ind), std::ref(d_ind_users), std::ref(d_row_size),
    std::ref(movies_names));

    b_recomend = recommend_button->connect("pressed", recomendar, std::ref(ids_movies), std::ref(movies_ratings),
    d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size,
    values, row_ind, col_ind, ind_users, row_size,
     std::ref(n_ratings), std::ref(n_users), COSINE, 0, 10, std::ref(movies_names));



    while (window.isOpen()){

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                cout<<"Key pressed"<<endl;
                if (event.key.code == sf::Keyboard::Escape)
                {
                  cout<<"n_ratings: "<<n_ratings<<"n_users: "<<n_users<<endl;
                  load_button->disconnectAll();
                  load_button->connect("pressed", load_dataset, std::ref(load_data), list_dbs->getSelectedItem(),
                  std::ref(n_ratings), std::ref(n_users), std::ref(values), std::ref(row_ind),
                  std::ref(col_ind), std::ref(ind_users), std::ref(row_size), std::ref(d_values),
                  std::ref(d_row_ind), std::ref(d_col_ind), std::ref(d_ind_users), std::ref(d_row_size),
                  std::ref(movies_names));

                  recommend_button->disconnectAll();

                  recommend_button->connect("pressed", recomendar, std::ref(ids_movies), std::ref(movies_ratings),
                  d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size,
                  values, row_ind, col_ind, ind_users, row_size,
                   std::ref(n_ratings), std::ref(n_users), COSINE, 4, 10, std::ref(movies_names));


                }
            }



            gui.handleEvent(event); // Pass the event to the widgets
        }


        window.clear();
        gui.draw(); // Draw all widgets
        window.display();
    }
}
