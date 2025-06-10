#include "node.h"
#include "parcel.h"
#include <iostream>
#include "custom_random.h"
#include <vector>

using namespace std;

//vector<Parcel>* Node::parcels = new vector<Parcel>();

Node::Node()
{
    node_id = 0;
    lastArrival = 0;
    connectedNodes = 0;
    capacity = 50;
    randomObj = new custom_random(5);
    parcels = new vector<Parcel>();
    deliveredTo =0;
    isInit=0;
    notDeliveredTo = 0;
}

void Node::init(int id, int seed, int parcel_mean)
{
    node_id = id;
    parcelMean = parcel_mean;
    //randomObj = new custom_random(seed);

}

int Node::getInit()
{
    return isInit;
}

void Node::setInit()
{
    isInit=1;
}

Node::~Node()
{
    delete randomObj;
    delete parcels;
}

int Node::parcelArrival(int currentTime, int arrivalmean)
{
    lastArrival = currentTime;
    //double u = randomObj->uniform();
    int count = randomObj->poissonInverse(arrivalmean, 10);


    //  generate the parcels
    for(int i = 0; i < count; i++)
    {
        generateParcel();
    }

    return count;
}

void Node::generateParcel()
{
    //  origin
    int origin = node_id;
    int destination = randomObj->randInt(1, 3);
    while(origin == destination)
    {
        destination = randomObj->randInt(1, 3);
    }
    double weight = randomObj->randDouble(0.1, 10) * (6); //max weight is 6kg

    //  create parcel
    Parcel p = Parcel(origin, destination, weight);

    if (parcels == nullptr)
    {
        std::cout << "parcels is null!" << std::endl;
    }

    //  append parcel to list
    parcels->push_back(p);
}

int Node::parcelCount()
{
    return parcels->size();
}

void Node::addDeliveredTo()
{
    deliveredTo+=1;
}

Parcel Node::popParcel()
{
    Parcel p = parcels->back();
    parcels->pop_back();
    return p;
}
