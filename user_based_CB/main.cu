#include "gui.h"


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

  inicializar_gui(std::ref(gui), std::ref(ids_movies), std::ref(movies_ratings),
  std::ref(d_values), std::ref(d_row_ind), std::ref(d_col_ind), std::ref(d_ind_users), std::ref(d_row_size),
  std::ref(values), std::ref(row_ind), std::ref(col_ind), std::ref(ind_users), std::ref(row_size),
  std::ref(n_ratings), std::ref(n_users), std::ref(movies_names), std::ref(load_data));

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
