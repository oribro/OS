#include <bits/stdc++.h> 
#include <pthread.h>
#include <omp.h>
  
#define INTERVAL 5000
using namespace std; 

int circle_points = 0, square_points = 0; 
  
int main() 
{ 
    double pi;
    int interval, i; 
    double rand_x, rand_y, origin_dist; 
  
    srand(time(NULL)); 

    #pragma omp parallel for
    for (i = 0; i < (INTERVAL * INTERVAL); i++) { 
        
        rand_x = double(rand() % (INTERVAL + 1)) / INTERVAL; 
        rand_y = double(rand() % (INTERVAL + 1)) / INTERVAL; 

        origin_dist = rand_x * rand_x + rand_y * rand_y; 
  
        if (origin_dist <= 1) 
            circle_points++; 
  
        square_points++; 
        cout << i << " ";
    } 
  
    pi = double(4 * circle_points) / square_points; 

    cout << circle_points << " " << square_points << " - " << pi << endl << endl; 

    cout << "\nFinal Estimation of Pi = " << pi; 
  
    return 0; 
} 
