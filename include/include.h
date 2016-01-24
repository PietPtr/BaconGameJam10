#ifndef INCLUDE_H
#define INCLUDE_H
#include <iostream>

struct Headline
{
    std::string text;
    int quality = -1;
    int truth = -1;
    //result if this headline is chosen
    int sympathy;
    int emotion;
    int money;
    int food;
};

struct News
{
    Headline headlines[3];
    std::string message;
    std::string endMessage; //can be empty
    //the cause of why this message appears
    int sympathy;
    int emotion;
    int money;
    int food;
};


#endif // INCLUDE_HZ
