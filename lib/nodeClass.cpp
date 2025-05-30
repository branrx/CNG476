#include "nodeClass.h"
#include "parcel.h"
#include <iostream>

using namespace std;

nodeClass::nodeClass(int id, int seed)
{
    node_id = id;
    lastArrival = 0;
    connectedNodes = 0;
    capacity = 50;
    randomObj = new random(seed);
}

nodeClass::~nodeClass()
{
    delete randomObj;
}

void nodeClass::parcelArrival(int currentTime)
{
    lastArrival = currentTime;
    double u = randomObj->uniform();
    int count = nodeClass::randomObj->poissonInverse(arrivalMean, u);
    cout << "Parcels arrived: " << count << endl;

    //  generate the parcels
    for(int i = 0; i < count; i++)
    {
        generateParcel();
    }
}

void nodeClass::generateParcel()
{
    //  origin
    int origin = node_id;
    int destination = randomObj->randInt(0, 5);
    while(origin == destination)
    {
        destination = randomObj->randInt(0, 5);
    }
    double weight = randomObj->randDouble(0.1, 10) * (6); //max weight is 6kg

    //  create parcel
    Parcel p = Parcel(origin, destination, weight);

    //  append parcel to list
    parcels.push_back(p);
}

int nodeClass::parcelCount()
{
    return parcels.size();
}