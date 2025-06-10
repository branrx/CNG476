/*
 * distribution_functions.h
 *
 *  Created on: May 29, 2025
 *      Author: branrx
 */

#ifndef DISTRIBUTION_FUNCTIONS_H_
#define DISTRIBUTION_FUNCTIONS_H_

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
    double uniformGenerator();

#endif /* LORAAPP_DISTRIBUTION_FUNCTIONS_H_ */
