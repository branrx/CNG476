#ifndef LORA_TRUCKGENERATOR_H_
#define LORA_TRUCKGENERATOR_H_

#include "custom_random.h"
#include <vector>
#include "truck.h"

class TruckGenerator
{
public:
    void Init(int);
    void Create();
    TruckGenerator();
    ~TruckGenerator();
    custom_random *randObj;
    static vector<Truck*> *trucks;
};

#endif /* LORA_TRUCKGENERATOR_H_ */
