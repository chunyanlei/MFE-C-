//
//  option.h
//  HW5_Q2
//
//  Created by 雷春妍 on 2020/2/3.
//  Copyright © 2020年 雷春妍. All rights reserved.
//

#ifndef option_h
#define option_h

class Option
{
protected:
    double strike;
    double timeToMaturity;
    double stockVolatility;
    double rf; // risk-free rate
    
public:
    Option(double K, double T, double sigma, double r):strike(K), timeToMaturity(T), stockVolatility(sigma), rf(r){};
    
    // 3. returns the exercise value of the option
    virtual double getExerciseValue(double s, double t)=0;
    
    // 4. returns the Black Scholes value of the option
    virtual double getBlackScholesValue(double s)=0;
    
    // 5. returns the value of the option using a binomial tree of depth N
    double getBinomialTreeValue(double s, int N);
    
    // 6.
    virtual double getValue(double s)=0;
    virtual ~Option(){};
};

class EuropeanOption: public Option
{
public:
    EuropeanOption(double K, double T, double sigma, double r):Option(K, T, sigma, r){};
    virtual double getExerciseValue(double s, double t){return 0;}
    virtual double getBlackScholesValue(double s){return 0;}
    virtual double getValue(double s);
    virtual ~EuropeanOption(){};
};

class AmericanOption: public Option
{
public:
    AmericanOption(double K, double T, double sigma, double r):Option(K, T, sigma, r){};
    virtual double getExerciseValue(double s, double t){return 0;}
    virtual double getBlackScholesValue(double s){return 0;}
    virtual double getValue(double s);
    virtual ~AmericanOption(){};
};

class KnockOutOption: public Option
{
protected:
    double barrier; // barrier
public:
    // 7.a
    KnockOutOption(double K, double T, double sigma, double r, double B):Option(K, T, sigma, r),barrier(B){};
    virtual double getExerciseValue(double s, double t){return 0;}
    virtual double getBlackScholesValue(double s){return 0;}
    virtual double getValue(double s);
    
    // 7.b
    bool isAccrossBarrier(double s);
    virtual ~KnockOutOption(){};
};

class EuropeanCall: public EuropeanOption
{
public:
    EuropeanCall(double K, double T, double sigma, double r):EuropeanOption(K, T, sigma, r){};
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class EuropeanPut: public EuropeanOption
{
public:
    EuropeanPut(double K, double T, double sigma, double r):EuropeanOption(K, T, sigma, r){};
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class AmericanCall: public AmericanOption
{
public:
    AmericanCall(double K, double T, double sigma, double r):AmericanOption(K, T, sigma, r){};
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class AmericanPut: public AmericanOption
{
public:
    AmericanPut(double K, double T, double sigma, double r):AmericanOption(K, T, sigma, r){};
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class KnockOutCall: public KnockOutOption
{
public:
    KnockOutCall(double K, double T, double sigma, double r, double B):KnockOutOption(K, T, sigma, r, B){};
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
    
};

class KnockOutPut: public KnockOutOption
{
public:
    KnockOutPut(double K, double T, double sigma, double r, double B):KnockOutOption (K, T, sigma, r, B){};
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);

};

#endif /* option_h */
