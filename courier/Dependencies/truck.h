#ifndef TRUCK_H_
#define TRUCK_H_

#include "custom_random.h"
#include "parcel.h"
#include <vector>

using namespace std;

class Truck
{
public:
    int id;             //  format truck - 3x,eg 31, 32
    int status;         //  0 idle, 1 loading
    int capacity;       //  carry limit
    int location;       //  where is it, 2x node, 10 base station
    int carrying;       //  current carry count
    int direction;      //  next location
    int eta ;           //  expected time of arrival at next location
    int isInit;     //  wait for parcels to arrive at node, then dispatch
    int loadingTime;    //  time taken at node
    bool isRepairing;
    int totalBrokenCount;
    int totalBrokenTime;
    int totalTravelTime;
    int totalLoadingTime;
    int totalUnLoadingTime;
    int totalIdleTime;
    vector<Parcel> *parcels;
    vector<int> *route;
    void Init(int, int);
    void addToRoute(int);
    void setChangeParcels();
    int getRouteSize();
    int getRouteNode(int);
    int getNextNode();
    int getInit();
    int getParcelCount();
    void setInit();
    Parcel getParcel(int);
    void unloadParcels(vector<int> *);
    void loadParcel(Parcel);
    void setRepairing(bool);
    Truck();
    ~Truck();
};


#endif /* TRUCK_H_ */
