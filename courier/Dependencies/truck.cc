#include "truck.h"
#include "custom_random.h"
#include "truckgenerator.h"
#include "parcel.h"
#include <vector>
//vector<Parcel>* Truck::parcels = new vector<Parcel>();
//vector<int>* Truck::route = new vector<int>();

Truck::Truck()
{
    Truck::id = 0;
    Truck::status = 0;
    Truck::isInit = 0;
    Truck::capacity = 0;
    Truck::location = 10;
    Truck::capacity = 0;
    Truck::carrying = 0;
    Truck::direction = 10;
    Truck::eta = 0;
    Truck::loadingTime = 0;
    Truck::parcels = new vector<Parcel>();
    Truck::route = new vector<int>();
    isRepairing = false;
    totalBrokenTime= 0;
    totalBrokenCount =0;
    totalTravelTime = 0;
    totalLoadingTime = 0;
    totalUnLoadingTime = 0;
    totalIdleTime = 0;
}

Truck::~Truck()
{

}

void Truck::Init(int truck_id, int truck_capacity)
{
    id = truck_id;
    capacity = truck_capacity;
}

void Truck::addToRoute(int node_id)
{
    route->push_back(node_id);
}

int Truck::getRouteSize()
{
    return route->size();
}

int Truck::getRouteNode(int index)
{
    return route->at(index);
}

int Truck::getNextNode()
{
    int id = route->back();
    route->pop_back();
    return id;
}

int Truck::getParcelCount()
{
    return parcels->size();
}

Parcel Truck::getParcel(int index)
{
    return parcels->at(index);
}

void Truck::loadParcel(Parcel p)
{
    parcels->push_back(p);
}

void Truck::unloadParcels(vector<int> *p)
{
    for(int i; i<p->size(); i++){
        int index = p->at(i);
        parcels->erase(parcels->begin()+index-i);
    }
}

void Truck::setInit()
{
    isInit=1;
}

int Truck::getInit()
{
    return isInit;
}

void Truck::setRepairing(bool val)
{
    isRepairing = val;
}

