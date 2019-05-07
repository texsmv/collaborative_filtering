#include "gui.h"
#include <thrust/sort.h>

bool compare3(const pair<float, int>&i, const pair<float, int>&j)
{
  if(i.first == j.first){
    return i.second < j.second;
  }
  return i.first > j.first;
}

int main(){



  // thrust::host_vector<float> key;
  // thrust::host_vector<int> values;
  // key.push_back(1);  values.push_back(12);
  // key.push_back(3);  values.push_back(1);
  // key.push_back(1);  values.push_back(1);
  // key.push_back(2);  values.push_back(2);
  // key.push_back(2);  values.push_back(1);
  // key.push_back(1);  values.push_back(4);
  // key.push_back(1);  values.push_back(3);
  //
  // thrust::device_vector<float> d_key= key;
  // thrust::device_vector<float> d_values = values;;
  //
  //
  // thrust::sort_by_key(d_values.begin(), d_values.begin() + 6, d_key.begin());
  //
  // thrust::copy(d_key.begin(), d_key.end(), key.begin());
  // thrust::copy(d_values.begin(), d_values.end(), values.begin());
  // thrust::copy(key.begin(), key.end(), d_key.begin());
  // thrust::copy(values.begin(), values.end(), d_values.begin());

  // for (size_t i = 0; i < 6; i++) {
  //   cout<<values[i]<<" "<<key[i]<<endl;
  // }



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
  map<int, float>* map_movies;


  bool load_data = true;
  vector<int> ids_movies;
  vector<float> movies_ratings;


  // User interface

  sf::RenderWindow window{{1300, 600}, "Window"};

  tgui::Gui gui{window}; // Create the gui and attach it to the window

  inicializar_gui(std::ref(gui), std::ref(ids_movies), std::ref(movies_ratings),
  std::ref(d_values), std::ref(d_row_ind), std::ref(d_col_ind), std::ref(d_ind_users), std::ref(d_row_size),
  std::ref(values), std::ref(row_ind), std::ref(col_ind), std::ref(ind_users), std::ref(row_size),
  std::ref(n_ratings), std::ref(n_users), std::ref(movies_names), std::ref(load_data), std::ref(map_movies));

  while (window.isOpen()){

      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
          // if (event.type == sf::Event::KeyPressed)
          // {
          //     if (event.key.code == sf::Keyboard::Escape)
          //     {
          //
          //
          //     }
          // }



          gui.handleEvent(event); // Pass the event to the widgets
      }


      window.clear();
      gui.draw(); // Draw all widgets
      window.display();
  }
}
