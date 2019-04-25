#include "math.h"
#include "tipos.h"


t_distance cosine(t_ratings* a, t_ratings* b){
    auto it1 = a->begin();
    auto it2 = b->begin();
    t_rating xy = 0;
    t_rating lxl = 0;
    t_rating lyl = 0;


    while(it1 != a->end() or it2 != b->end()){
        if(it1 != a->end() && it2 != b->end()){
            if(it1->first == it2->first){
                xy += it1->second * it2->second;
                lxl += pow(it1->second,2);
                lyl += pow(it2->second,2);
                it1++; it2++;
            }else if(it1->first < it2->first){
                lxl += pow(it1->second,2);
                it1++;
            }else{
                lyl += pow(it2->second,2);
                it2++;
            }
        }else{
                if(it1 != a.end()){
                    lyl += pow(it1->second,2);
                    it1++;
                }else{
                    lxl += pow(it2->second,2);
                    it2++;
                }
        }
    }
    return (lxl == 0 || lyl == 0)? 0 : (xy / (sqrt(lxl)*sqrt(lyl)));
}


t_distance euclidean(t_ratings* a, t_ratings* b){
    auto it1 = a->begin();
    auto it2 = b->begin();
    t_distance sum = 0;
    while(it1 != a->end() && it2 != b->end()){
        if(it1->first == it2->first){
            sum += pow(it1->second - it2->second,2);
            it1++; it2++;
        }else if(it1->second < it2->second)
            it1++;
        else
            it2++;
    }
    return sqrt(sum);
}

t_distance pearson(t_ratings* a, t_ratings* b){
    auto it1 = a->begin();
    auto it2 = b->begin();
    t_distance xy = 0;
    t_distance x = 0;
    t_distance y = 0;
    t_distance x_2 = 0;
    t_distance y_2 = 0;
    int n = 0;
    while(it1 != a->end() && it2 != b->end()){
        if(it1->first == it2->first){
            xy += it1->second * it2->second;
            x += it1->second;
            y += it2->second;
            x_2 += pow(it1->second,2);
            y_2 += pow(it2->second,2);
            n++;
            it1++; it2++;
        }else if(it1->second < it2->second)
            it1++;
        else
            it2++;
    }
    t_distance n1 = xy - (( x * y )/n);
    t_distance d1 = sqrt(x_2 -(pow(x,2)/n));
    t_distance d2 = sqrt(y_2 -(pow(y,2)/n));
    return (d1 == 0 || d2 == 0) ? 0 : n1 / (d1 * d2);
}
