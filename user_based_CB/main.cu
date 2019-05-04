#include <TGUI/TGUI.hpp>
#include "scripts.h"
#include <iostream>
#include "structures.h"
#include "distances.h"
#include "cud_sparse_oper.h"
#include "recomender.h"

// using namespace std;

void recomendar(tgui::Gui& gui,vector<int>& ids_movies, vector<float>& movies_ratings, float*& d_values, int*& d_row_ind, int*& d_col_ind, int*& d_ind_users, int*& d_row_size, float*& values, int*& row_ind, int*& col_ind, int*& ind_users, int*& row_size, int& n_ratings, int& n_users, map<int, string>& movies_names){
  // cout<<"recomendar: "<<a<<endl;
  ids_movies.clear();
  movies_ratings.clear();
  // movies_names.empty();


  tgui::EditBox::Ptr editBox_id_user = gui.get<tgui::EditBox>("editbox_id_user");
  tgui::EditBox::Ptr editBox_k = gui.get<tgui::EditBox>("editbox_k");
  tgui::ComboBox::Ptr comboBox_metrics = gui.get<tgui::ComboBox>("comboBox_metrics");
  tgui::ListBox::Ptr list_movies = gui.get<tgui::ListBox>("list_movies");


  int user_pos = atoi(std::string(editBox_id_user->getText()).c_str()) - 1;
  int k = atoi(std::string(editBox_k->getText()).c_str());
  int measure;
  list_movies->removeAllItems();

  if(comboBox_metrics->getSelectedItem() == "Euclidean")
    measure = EUCLIDEAN;
  else if(comboBox_metrics->getSelectedItem() == "Pearson")
    measure = PEARSON;
  else
    measure = COSINE;


  cout<<n_ratings<<" "<<n_users<<endl;
  reloj r;
  r.start();
  // k_ordered_recomendations(ids_movies, movies_ratings, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, n_ratings, n_users, COSINE, user_pos, 10, movies_names);
  k_recomendations(ids_movies, movies_ratings, d_values, d_row_ind, d_col_ind, d_ind_users, d_row_size, values, row_ind, col_ind, ind_users, row_size, n_ratings, n_users, measure, user_pos, k);
  r.stop();
  cout<<"Tiempo de recomendacion: "<<r.time()<<"ms"<<endl;
  for (size_t i = 0; i < ids_movies.size(); i++) {
    list_movies->addItem(movies_names[ids_movies[i]] + " -> " + to_string(movies_ratings[i]));
    cout<<movies_names[ids_movies[i]]<<" "<<movies_ratings[i]<<endl;
  }
}




void load_dataset(tgui::ListBox::Ptr list_dbs, bool& load, int& n_ratings, int& n_users, float*& values, int*& row_ind, int*& col_ind, int*& ind_users, int*& row_size, float*& d_values, int*& d_row_ind, int*& d_col_ind, int*& d_ind_users, int*& d_row_size, map<int, string>& movies_names){
  string option = list_dbs->getSelectedItem();
  cout<<option<<endl;

  if(load && option != ""){
    cout<<"In load function"<<endl;
    load = false;
    reloj r;
    // cout<<"Opcion: "<<load<<endl;
    if(option == "Movie Lens 27M"){
      // cout<<option<<endl;
      n_ratings = 27753444;
      n_users = 283228;
      read_ML_movies("../databases/ml-latest/movies.csv", movies_names, true);
      r.start();
      read_ML_ratings("../databases/ml-latest/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);
      r.stop();
      cout<<"Tiempo de carga: "<<r.time()<<endl;
    }
    else if(option == "Movie Lens 20M"){
      n_ratings = 20000263;
      n_users = 138493;
      read_ML_movies("../databases/ml-20m/movies.csv", movies_names, true);
      r.start();
      read_ML_ratings("../databases/ml-20m/ratings.csv", n_ratings, n_users, true, values, row_ind, col_ind, ind_users, row_size);
      r.stop();
      cout<<"Tiempo de carga: "<<r.time()<<endl;
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

    tgui::Theme theme{"Black.txt"};
    tgui::Theme::setDefault(&theme);

    auto picture = tgui::Picture::create("iron_man.jpg");
    picture->setSize({"100%", "100%"});
    gui.add(picture);


    // tgui::Texture texture("iron_man.jpg");
    //
    // tgui::Picture picture(texture);

    // picture->load("iron_man.jpg");

    tgui::ListBox::Ptr list_dbs = tgui::ListBox::create();
    tgui::ListBox::Ptr list_movies = tgui::ListBox::create();

    tgui::Button::Ptr load_button = tgui::Button::create();
    tgui::Button::Ptr recommend_button = tgui::Button::create();
    tgui::Button::Ptr predict_button = tgui::Button::create();

    tgui::ComboBox::Ptr comboBox_metrics = tgui::ComboBox::create();

    tgui::EditBox::Ptr editBox_id_user = tgui::EditBox::create();
    tgui::EditBox::Ptr editBox_id_item = tgui::EditBox::create();
    tgui::EditBox::Ptr editBox_k = tgui::EditBox::create();

    tgui::Label::Ptr label_dataset = tgui::Label::create();
    tgui::Label::Ptr label_recommend = tgui::Label::create();
    tgui::Label::Ptr label_predict = tgui::Label::create();
    tgui::Label::Ptr label_id_user = tgui::Label::create();
    tgui::Label::Ptr label_id_item = tgui::Label::create();
    tgui::Label::Ptr label_k = tgui::Label::create();
    tgui::Label::Ptr label_metric = tgui::Label::create();

    gui.add(load_button, "LoadButton");
    gui.add(recommend_button, "RecommendButton");
    gui.add(predict_button, "ProyectButton");
    gui.add(editBox_id_user, "editbox_id_user");
    gui.add(editBox_id_item, "editbox_id_item");
    gui.add(editBox_k, "editbox_k");

    gui.add(list_dbs, "list_dataset");
    gui.add(list_movies, "list_movies");

    gui.add(label_dataset, "label_dataset");
    gui.add(label_id_user, "label_id_user");
    gui.add(label_id_item, "label_id_item");
    gui.add(label_k, "label_k");
    gui.add(label_metric, "label_metrics");
    gui.add(label_recommend, "label_recommend");
    gui.add(label_predict, "label_predict");

    gui.add(comboBox_metrics, "comboBox_metrics");



    label_dataset->setText("Elija dataset");
    label_dataset->setPosition("10%", "10%");

    list_dbs->addItem("Movie Lens 27M");
    list_dbs->addItem("Movie Lens 20M");
    list_dbs->setPosition("label_dataset.left", "label_dataset.top + 30");

    load_button->setText("Cargar dataset");
    load_button->setPosition("label_dataset.left", "label_dataset.top + 190");

    label_id_user->setText("Id del usuario:");
    label_id_user->setPosition("10%", "50%");

    label_k->setText("Knn:");
    label_k->setPosition("label_id_user.left", "label_id_user.top + 50");

    label_metric->setText("Metrica:");
    label_metric->setPosition("label_k.left", "label_k.top + 50");

    // load_button->setSize();


    editBox_id_user->setPosition("25%", "label_id_user.top");
    editBox_id_user->setText("4");

    editBox_k->setPosition("25%", "label_k.top");
    editBox_k->setText("10");

    comboBox_metrics->setPosition("25%", "label_metrics.top");
    comboBox_metrics->addItem("Euclidean");
    comboBox_metrics->addItem("Pearson");
    comboBox_metrics->addItem("Cosine");
    comboBox_metrics->setSelectedItem("Euclidean");


    label_recommend->setPosition("55%", "10%");
    label_recommend->setText("Recomendacion");

    recommend_button->setPosition("label_recommend.right + 70", "label_recommend.top");
    recommend_button->setText("Recomendar");
    list_movies->setSize(300, 280);
    list_movies->setPosition("label_recommend.left", "label_recommend.top + 40");


    label_predict->setPosition("55%", "65%");
    label_predict->setText("Prediccion");

    predict_button->setPosition("label_predict.right + 70", "label_predict.top");
    predict_button->setText("Proyectar");
    label_id_item->setPosition("label_predict.left", "label_predict.top + 40");
    label_id_item->setText("Id del item:");
    editBox_id_item->setPosition("label_id_item.right + 65", "label_id_item.top");





    load_button->connect("pressed", load_dataset, list_dbs, std::ref(load_data),
    std::ref(n_ratings), std::ref(n_users), std::ref(values), std::ref(row_ind),
    std::ref(col_ind), std::ref(ind_users), std::ref(row_size), std::ref(d_values),
    std::ref(d_row_ind), std::ref(d_col_ind), std::ref(d_ind_users), std::ref(d_row_size),
    std::ref(movies_names));

    recommend_button->connect("pressed", recomendar, std::ref(gui), std::ref(ids_movies), std::ref(movies_ratings),
    std::ref(d_values), std::ref(d_row_ind), std::ref(d_col_ind), std::ref(d_ind_users), std::ref(d_row_size),
    std::ref(values), std::ref(row_ind), std::ref(col_ind), std::ref(ind_users), std::ref(row_size),
    std::ref(n_ratings), std::ref(n_users), std::ref(movies_names));

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
                  // load_button->disconnectAll();

                  // recommend_button->disconnectAll();



                }
            }



            gui.handleEvent(event); // Pass the event to the widgets
        }


        window.clear();
        gui.draw(); // Draw all widgets
        window.display();
    }
}
