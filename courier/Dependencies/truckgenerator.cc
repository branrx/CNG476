#include "custom_random.h"
#include "truck.h"
#include <vector>
#include <iostream>
#include "truckgenerator.h"

vector<Truck*>* TruckGenerator::trucks = new vector<Truck*>(30);

TruckGenerator::TruckGenerator()
{
    randObj = new custom_random(5);
    trucks = new vector<Truck*>(30);
}

TruckGenerator::~TruckGenerator()
{
    delete randObj;
    delete trucks;
}

void TruckGenerator::Init(int seed)
{
    randObj = new custom_random(seed);
}

void TruckGenerator::Create()
{
    //  create truck
    /*Truck *truck = new Truck(0, 20);

    if (trucks == nullptr)
    {
        std::cout << "trucks is null!" << std::endl;
    }
    //  append truck to list
    trucks->push_back(truck);
    delete truck;
    //delete truck;
    std::cout << "truck added." << std::endl;*/
}
