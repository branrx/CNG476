#ifndef NODE_H_
#define NODE_H_

#include "parcel.h"
#include <vector>
#include "custom_random.h"

using namespace std;

class Node{
public:
    int node_id;
    int connectedNodes;
    int lastArrival;
    int capacity;
    int valh;
    int deliveredTo;
    int notDeliveredTo;
    int isInit;
    int parcelMean;

    Node();
    void generateParcel();
    Parcel popParcel();
    void addDeliveredTo();
    void setInit();
    int getInit();
    int parcelCount();
    int parcelArrival(int currentTime, int arrivalMean);
    void init(int, int, int);
    custom_random *randomObj;
    vector<Parcel> *parcels;
    ~Node();
};

#endif /* NODE_H_ */
