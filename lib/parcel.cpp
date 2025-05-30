#include "parcel.h"

Parcel::Parcel(int origin, int destination, double weight)
{
    //  set parcel properties
    Parcel::origin = origin;
    Parcel::destination = destination;
    Parcel::delivery_status = 0;
    Parcel::weight = weight;
}

Parcel::~Parcel()
{

}
