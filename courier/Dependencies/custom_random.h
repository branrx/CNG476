#ifndef CUSTOM_RANDOM_H_
#define CUSTOM_RANDOM_H_

class custom_random
{
public:
    int seed;
    custom_random(int);
    void init(int);
    double uniform();
    int randInt(int min, int max);
    double randDouble(double min, int max);
    int poissonInverse(float lambda, double n);
    bool biasedBinary(double);
    double inverseExponential(float, float, double);
};

#endif /* CUSTOM_RANDOM_H_ */
