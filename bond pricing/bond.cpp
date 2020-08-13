//
//  bond.cpp
//  hw5
//
//  Created by 雷春妍 on 2020/2/2.
//  Copyright © 2020年 雷春妍. All rights reserved.
//

#include "bond.h"

const double Epsilon = 1e-10;

bool isZero(double x)
{
    return fabs(x) < Epsilon;
}

// 2.
double Bond::getPrice(double YTM)
{
    double price = principal / pow((1 + YTM),timeToMaturity);
    for(int i = 0; timeToMaturity - i > 0; i++)
    {
        price += coupon*(1 / pow((1 + YTM), timeToMaturity - i));
    }
    return price;
}

// 3.
// Use bisection root finding method
double Bond::getYTM(double price)
{
    int iter = 100; // # of max iterations
    
    double x0 = -10;
    double x1 = 10;
    
    double f2 = 1;
    double x2 = 0;
    int cnt = 0;
    
    while(!isZero(f2) && cnt < iter)
    {
        x2 = 0.5*(x0 + x1);
        f2 = price - getPrice(x2);
        if(f2 < 0)
            x0 = x2;
        else
            x1 = x2;
        cnt++;
    }
    return x2;
}

// 4.
double Bond::estimateModifiedDuration(double YTM)
{
    double dur = 100 * (getPrice(YTM) - getPrice(YTM + 0.01)) /getPrice(YTM);
    return dur;
}

// 5.
double Bond::estimateSimulatedPrice(double hazardRate, double r, int numSimulations)
{
    // simulate a default time
    
    double sum = 0;
    mt19937 mt_rand(time(0));
    uniform_real_distribution<double> dis_unif(0,1);
    
    for(int sim = 0; sim < numSimulations; sim++)
    {
        
        double u = dis_unif(mt_rand);
        
        double defaultTime = - log(1 - u)/hazardRate;
        
        // calculate the present value of the bond's cash flows (discounted by r)
        
        double bondPrice = 0;
        
        if(defaultTime > timeToMaturity)
        {
            bondPrice += principal * exp(- r * timeToMaturity);
            for(int i = 0; timeToMaturity - i > 0; i++)
            {
                bondPrice += coupon * exp(- r * (timeToMaturity - i));
            }
        }
        else
        {
            double tmp = timeToMaturity - floor(timeToMaturity) + floor(defaultTime);
            double lastT = tmp > defaultTime? tmp-1 : tmp;
            for(int i = 0; lastT - i >= 0; i++)
            {
                bondPrice += coupon * exp(- r * (timeToMaturity - i));
            }
        }
        sum += bondPrice;
    }
    return sum/numSimulations;
}
