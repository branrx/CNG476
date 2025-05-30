#ifndef NODECLASS_H
#define NODECLASS_H

#include "random.h"
#include "parcel.h"
#include <vector>
using namespace std;
class nodeClass{
public:
    int node_id;
    int connectedNodes;
    int lastArrival;
    int arrivalMean = 8;    // t * count, eg 1*8
    vector<Parcel> parcels;
    int capacity;

    nodeClass(int, int);
    void generateParcel();
    int parcelCount();
    void parcelArrival(int currentTime);
    random *randomObj;
    ~nodeClass();
};

#endif