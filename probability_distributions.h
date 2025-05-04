#ifndef UNTITLED_PROBABILITY_DISTRIBUTIONS_H
#define UNTITLED_PROBABILITY_DISTRIBUTIONS_H
int factorial(int);
long double binomial(int, int, float);
long double binomialCDF(int, int, float);
long double geometric(float, int);
long double geometricCDF(float, int);
long double exponential(float);
long double exponentialCDF(float);
long double poisson(float, float);
int poissonCDF(float, double);
long long int mixedLCG(long long int);
long long int mixedLCGiter(int, long long);
long long int multiplicativeLCG(long long int);
long long int multiplicativeLCGiter(int, long long);
double inverseExponential(float, float, double);
void inverseExponentialiter(float, float, int);
void poissonInverse(float, int);
#endif //UNTITLED_PROBABILITY_DISTRIBUTIONS_H
