#ifndef RANDOM_H
#define RANDOM_H

class random
{
public:
    int seed;
    random(int );
    double uniform();
    int randInt(int min, int max);
    double randDouble(double min, int max);
    int poissonInverse(float lambda, int n);
};

#endif //RANDOM_H
