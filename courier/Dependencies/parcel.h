#ifndef PARCEL_H_
#define PARCEL_H_


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

#endif /* PARCEL_H_ */
