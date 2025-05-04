#include <stdio.h>
#include <stdlib.h>
#include "probability_distributions.h"

int main(void)
{
    srand(2);
    float p=0.5f;
    int n=3, k=4;
    //binomial(n, k, p);

    //  Binomial CDF
    long double result = binomialCDF(4, 3, 0.5f);
    printf("\nBinomial CDF: %.3Lf", result);

    //  Geometric
    result = geometric(0.5f, 5);
    printf("\nGeometric PMF: %.3Lf", result);

    //  Geometric CDF
    result = geometricCDF(0.5f, 5);
    printf("\nGeometric CDF: %.3Lf", result);

    //  Exponential PDF
    result = exponential(10);
    printf("\nExponential: %.3Lf", result);

    //  Exponential CDF
    result = exponentialCDF(10);
    printf("\nExponential CDF CDF: %.3Lf", result);

    //  Poisson
    result = poisson(6, 5);
    printf("\nPoisson: %.3Lf", result);

    //  Poisson
    int ans = poissonCDF(4*2, 0.7f);
    printf("\nInverse Poisson: %d", ans);

    //  Mixed LCG
    long long int mlcg = mixedLCG(2);
    printf("\nMixed LCG: %lld", mlcg);

    //  Mixed LCG iterative
    mlcg = mixedLCGiter(6, 2);
    printf("\nMixed LCG: %lld", mlcg);

    //  Multiplicative LCG iterative
    mlcg = multiplicativeLCGiter(6, 2);
    printf("\nMultiplicative LCG: %lld", mlcg);

    //  Inverse Exponential
    float iExp = inverseExponential(4, 1, 0.3f);
    printf("\nInverse Exponential: %f", iExp);

    //inverseExponentialiter(2, 1, 10);

    poissonInverse(4,10);
}