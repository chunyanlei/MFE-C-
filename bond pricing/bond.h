//
//  bond2.h
//  hw5
//
//  Created by 雷春妍 on 2020/2/3.
//  Copyright © 2020年 雷春妍. All rights reserved.
//

#ifndef bond_h
#define bond_h

#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
using namespace std;


class Bond
{
private:
    double principal;
    double coupon; // paid annually
    double timeToMaturity;
    
public:
    // 1. Constuctor
    Bond(double principal0, double coupon0, double timeToMaturity0):principal(principal0),coupon(coupon0),timeToMaturity(timeToMaturity0){}
    double getPrice(double YTM);
    double getYTM(double price);
    double estimateModifiedDuration(double YTM);
    double estimateSimulatedPrice(double hazardRate, double r, int numSimulations);
};


#endif /* bond_h */
