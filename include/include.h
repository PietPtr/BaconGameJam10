#ifndef INCLUDE_H
#define INCLUDE_H
#include <iostream>

struct Headline
{
    std::string text;
    int quality = -1;
    int truth = -1;
};

struct News
{
    Headline headlines[3];
    std::string message;
};


#endif // INCLUDE_HZ
