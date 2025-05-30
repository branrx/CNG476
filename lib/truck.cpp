#include "truck.h"

Truck::Truck(int t_id, int t_capacity)
{
    id = t_id;
    status = 0;
    location = 10;
    capacity = t_capacity;
    carrying = 0;
    direction = 10;
    arrivalTime = 0;
    loadingTime = 0;
}

Truck::~Truck()
{}

