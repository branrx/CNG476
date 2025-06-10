#include "custom_random.h"
#include <math.h>
#include <cstdlib>
#include <iostream>

using namespace std;

custom_random::custom_random(int val)
{
    srand(val);
}

void custom_random::init(int seed)
{
    srand(seed);
}

double custom_random::uniform()
{
    return (double)(rand()%100) / 100.0;
}

int custom_random::randInt(int min, int max)
{
    return (rand()%max)+min;
}

bool custom_random::biasedBinary(double bias)
{

    double d = (double)(rand()%100) / 100.0;
    if(pow(d, bias)<0.5)
    {
        return false;
    }
    return true;
}

//  inverse exponential distribution
double custom_random::inverseExponential(float count, float t, double u)
{
    double lambda = count/t;
    return -(1.0f/lambda) * log(1-u);
}

double custom_random::randDouble(double min, int max)
{
    return ((rand()%max)+min) / max;
}

//  draw from a poisson distribution
//  lambda is the average = t * count
//  n is the probability
int custom_random::poissonInverse(float lambda, double n)
{
    double L = pow(2.7182, -1.0*(lambda));
    double p = 1;
    int x = 0;

    p = 1;
    for (int j = 0; p > L; j++)
    {
        double u = ((double)rand() / RAND_MAX);
        p *= u;
        x = j;
    }
    //cout << x << endl;
    return x;
}
