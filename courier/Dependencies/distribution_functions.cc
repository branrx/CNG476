#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "distribution_functions.h"
#include <stdlib.h>
#include <time.h>

double uniformGenerator()
{
    return (double)(rand()%100)/100;
}

//  computes factorial
int factorial(int a)
{
    int ans;

    if(a==0)
    {
        return 1;
    }   else
    {
        ans = a * factorial(a-1);
    }
    return ans;
}

//  computes prob of occurence of an event in a set of discrete events
long double binomial(int n, int k, float p)
{
    // formula -> (n;k) * p^k * (1-p)^n-k
    //  where a = (n;k) == n!/k!(n-k)!
    //  b = p^k, c = (1-p)^n-k

    if(factorial(n)<=0 || n-k<0)
    {
        printf("binomial prob distr: 0");
        return 0.0;
    }
    long double a = (float)factorial(n)/ (factorial(k)* factorial(n-k));
    long double b = pow(p, k);
    long double c = pow((1-p), n-k);

    long double result = a * b * c;

    return result;
}

//  computes cumulative prob of occurence of an event in a set of discrete events
long double binomialCDF(int count, int n, float p)
{
    long double result=0;
    for (int i=-1; i < count; ++i)
    {
        result+= binomial(n, i+1, p);
    }
    return result;
}

//  geometric: when will we get success of failure DISCRETE
long double geometric(float p, int k)
{
    if(k<=0)
    {
        return 0.0;
    }
    //  formula: (1-p)^k-1.p
    return powl((1-p), k-1)*p;
}

//  computes cumulative distributions of when will we get success of failure DISCRETE
long double geometricCDF(float p, int k)
{
    //  formula: 1-(1-p)^k
    return 1-powl((1-p), k);
}

//  computes odds based on exponetial distribution
long double exponential(float x)
{
    //  formula: lambda.e^-(lambda.x)
    //  where lambda = 1/x

    float lambda = 1.0f/x;
    return lambda* exp(-(lambda*x));
}

//  draws from an exponential distribution
long double exponentialCDF(float x)
{
    //  formula: 1-e^-(lambda.x)
    //  where lambda = 1/x

    float lambda = 1.0f/x;
    return 1 - exp(-(lambda*x));
}

//  computes probability of k events over a period of time
long double poisson(float lambda, float k)
{
    //  formula: e^-lambda*t * (lambda)^n/n!
    //  a = e^-lambda*t, b = (lambda*t)^n, c =n!
    //  where lambda is the mean
    //float lambda = 1/n;

    long double a = exp(-lambda);
    long double b = powl(lambda,k);
    long double c = factorial(k);

    return (a * b) / c;

}

//  draws from a poisson distribution
int poissonCDF(float lambda, double p)
{
    long double result=0;
    int X = 0;
    for (int i = 0; result < p; ++i)
    {
        result += poisson(lambda, (float)i);
        X = i;
    }
    return X;
}

//  LCGs: generate random numbers
//  Mixed:  generates a random number
long long int mixedLCG(long long int seed)
{
    long double a = powl(5, 1);
    long double c = 3;
    long long int m = 7;

    //  formula: a*xn-1 + c % m
    return (long long int)((a * seed) + c) % m;
}

//  Mixed iterative lcg:    generates random numbers
long long int mixedLCGiter(int iter, long long seed)
{
    printf("\nx: %d = %lld", 0, seed);
    for (int i = 0; i < iter; ++i)
    {
        seed = mixedLCG(seed);
        printf("\nx: %d = %lld", i+1, seed);
    }
    return seed;
}

//  Multiplicative:   generates a random number
long long int multiplicativeLCG(long long int seed)
{
    long double a = powl(5, 1);
    long double c = 3;
    long long int m = 7;

    //  formula: a*xn-1 % m
    return (long long int)((a * seed)) % m;
}

//  Multiplicative iterative lcg:   generates random numbers
long long int multiplicativeLCGiter(int iter, long long seed)
{
    printf("\nx: %d = %lld", 0, seed);
    for (int i = 0; i < iter; ++i)
    {
        seed = multiplicativeLCG(seed);
        printf("\nx: %d = %lld", i+1, seed);
    }
    return seed;
}

//  inverse exponential distribution
double inverseExponential(float count, float t, double u)
{
    double lambda = count/t;
    return -(1.0f/lambda) * log(1-u);
}

//  draw from a cumulative exponential distribution
//  example: for odds between a and b
void inverseExponentialiter(float count, float t, int n)
{
    for (int i = 0; i < n; ++i)
    {
        double u = (double)rand()/RAND_MAX;
        printf("\nrand: %lf", u);
        double arrivalTime = inverseExponential(count, t, u);
        printf("\nExponetial inverse: Person %d arrival: %.4f", i+1, arrivalTime);
    }
}

//  draw from a poisson distribution
void poissonInverse(float lambda, int n)
{
    double L = expl(-lambda);
    double p = 1;
    int x = 0;
    for (int i = 0; i < n; i++)
    {
        p = 1;
        for (int j = 0; p > L; j++)
        {
            double u = ((double)rand() / RAND_MAX);
            p *= u;
            x = j;
        }
        printf("\nPoisson Inverse: Time %d : %d arrivals", i+1, x);
    }
}
