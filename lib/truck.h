#ifndef CNG476_TRUCK_H
#define CNG476_TRUCK_H

#include "parcel.h"
#include <vector>

using namespace std;

class Truck
{
public:
    int id;             //  format truck - 3x,eg 31, 32
    int status;         //  0 idle, 1 loading
    int location;       //  where is it, 2x node, 10 base station
    int capacity;       //  carry limit
    int carrying;       //  current carry count
    int direction;      //  next location
    int arrivalTime;    //  when will it arrive at next location
    int loadingTime;    //  time taken at node
    vector<Parcel> parcels;

    Truck(int, int);
    ~Truck();
};

#endif //CNG476_TRUCK_H
