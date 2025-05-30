#ifndef CNG476_PARCEL_H
#define CNG476_PARCEL_H

class Parcel
{
public:
    int origin;
    int destination;
    int delivery_status;
    double weight;

    Parcel(int, int, double);
    ~Parcel();
};

#endif //CNG476_PARCEL_H
