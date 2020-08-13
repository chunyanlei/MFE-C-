//
//  option.cpp
//  HW5_Q2
//
//  Created by 雷春妍 on 2020/2/3.
//  Copyright © 2020年 雷春妍. All rights reserved.
//

#include "option.h"
#include <cmath>
#include <vector>
using namespace std;

double normalCDF(double x)
{
    return erfc(-x/sqrt(2))/2;
}

double max(double a, double b)
{
    return a > b ? a : b;
}


double EuropeanCall::getExerciseValue(double s, double t)
{
    // European options can only be exercised at maturity
    if(t == timeToMaturity)
        return(max(0, s - strike));
    
    return 0;
}

double EuropeanPut::getExerciseValue(double s, double t)
{
    // European options can only be exercised at maturity
    if(t == timeToMaturity)
        return(max(0, strike - s));
    
    return 0;
}

double AmericanCall::getExerciseValue(double s, double t)
{
    return(max(0, s - strike));
}


double AmericanPut::getExerciseValue(double s, double t)
{
    return(max(0, strike - s));
}

double KnockOutCall::getExerciseValue(double s, double t)
{
    if((t == timeToMaturity)&& (!isAccrossBarrier(s)))
        return(max(0, s - strike));
    
    return 0;
}

double KnockOutPut::getExerciseValue(double s, double t)
{
    if((t == timeToMaturity)&& (!isAccrossBarrier(s)))
        return(max(0, strike - s));
    
    return 0;
}


double EuropeanCall::getBlackScholesValue(double s)
{
    double d1 = 1/(stockVolatility * sqrt(timeToMaturity)) * (log(s/strike) + (rf + stockVolatility*stockVolatility/2)*timeToMaturity);
    double d2 = d1 - stockVolatility * sqrt(timeToMaturity);
    double callPrice = normalCDF(d1) * s - normalCDF(d2) * strike *exp(-rf*timeToMaturity);
    return callPrice;
}

double EuropeanPut::getBlackScholesValue(double s)
{
    double d1 = 1/(stockVolatility * sqrt(timeToMaturity)) * (log(s/strike) + (rf + stockVolatility*stockVolatility/2)*timeToMaturity);
    double d2 = d1 - stockVolatility * sqrt(timeToMaturity);
    double putPrice = normalCDF(-d2) * strike * exp(-rf*timeToMaturity) - normalCDF(-d1) * s;
    return putPrice;
}

double AmericanCall::getBlackScholesValue(double s)
{
    double d1 = 1/(stockVolatility * sqrt(timeToMaturity)) * (log(s/strike) + (rf + stockVolatility*stockVolatility/2)*timeToMaturity);
    double d2 = d1 - stockVolatility * sqrt(timeToMaturity);
    double callPrice = normalCDF(d1) * s - normalCDF(d2) * strike *exp(-rf*timeToMaturity);
    return callPrice;
}

double AmericanPut::getBlackScholesValue(double s)
{
    double d1 = 1/(stockVolatility * sqrt(timeToMaturity)) * (log(s/strike) + (rf + stockVolatility*stockVolatility/2)*timeToMaturity);
    double d2 = d1 - stockVolatility * sqrt(timeToMaturity);
    double putPrice = normalCDF(-d2) * strike * exp(-rf*timeToMaturity) - normalCDF(-d1) * s;
    return putPrice;
}

double KnockOutCall::getBlackScholesValue(double s)
{
    double d1 = 1/(stockVolatility * sqrt(timeToMaturity)) * (log(s/strike) + (rf + stockVolatility*stockVolatility/2)*timeToMaturity);
    double d2 = d1 - stockVolatility * sqrt(timeToMaturity);
    double callPrice = normalCDF(d1) * s - normalCDF(d2) * strike *exp(-rf*timeToMaturity);
    return callPrice;
}

double KnockOutPut::getBlackScholesValue(double s)
{
    double d1 = 1/(stockVolatility * sqrt(timeToMaturity)) * (log(s/strike) + (rf + stockVolatility*stockVolatility/2)*timeToMaturity);
    double d2 = d1 - stockVolatility * sqrt(timeToMaturity);
    double putPrice = normalCDF(-d2) * strike * exp(-rf*timeToMaturity) - normalCDF(-d1) * s;
    return putPrice;
}

double Option::getBinomialTreeValue(double s, int N)
{
    double deltaT = timeToMaturity / N;
    double up = exp(stockVolatility * sqrt(deltaT));
    
    double p0 = (up - exp(-rf * deltaT)) / (up * up - 1);
    double p1 = exp(-rf * deltaT)  - p0;
    
    // hold tree node values
    vector<double> p;
    
    // initial values at time T
    for(int i = 0; i <= N; i++)
    {
        double spot = s * pow(up, 2*i-N);
        p.push_back(getExerciseValue(spot, timeToMaturity));
        if(p[i] < 0)
            p[i] = 0;
    }
    
    
    double t = timeToMaturity;
    // move to earlier times
    for(int j = N-1; j >= 0; j--)
    {
        t = t - deltaT;
        for(int i = 0; i <= j; i++)
        {
            double spot = s * pow(up, 2*i-j);
            double exercise = getExerciseValue(spot, t);
            p[i] = p0 * p[i+1] + p1 * p[i];
            
            // check for early exercise
            if(p[i] < exercise)
                p[i] = exercise;
        }
    }
    return p[0];
    
}

// return B-S value
double EuropeanOption::getValue(double s)
{
    return this->getBlackScholesValue(s);
}

double AmericanOption::getValue(double s)
{
    return this->getBinomialTreeValue(s, 250);
}

double KnockOutOption::getValue(double s)
{
    return this->getBinomialTreeValue(s, 250);
}

bool KnockOutOption::isAccrossBarrier(double s)
{
    if(strike < barrier) // up-and-out
        return s > barrier ? 1: 0;
    // down-and-out
    return s < barrier ? 1 : 0;
}




