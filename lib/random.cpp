#include "random.h"
#include <math.h>
#include <cstdlib>

random::random(int val)
{
    seed = val;
    srand(seed);
}

double random::uniform()
{
    return (rand()%100) / 100.0;
}

int random::randInt(int min, int max)
{
    return (rand()%max)+min;
}

double random::randDouble(double min, int max)
{
    return ((rand()%max)+min) / max;
}

//  draw from a poisson distribution
//  lambda is the average = t * count
//  n is the probability
int random::poissonInverse(float lambda, int n)
{
    double L = expl(-lambda);
    double p = 1;
    int x = 0;
    p = 1;
    for (int j = 0; p > L; j++)
    {
        double u = ((double)rand() / RAND_MAX);
        p *= u;
        x = j;
    }
    return x;
}