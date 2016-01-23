#include "News.h"

News::News()
{

}

void News::initiate(std::string message, std::string* _headlines, int* quality, int* truth)
{
    this->message = message;
    this->headlines = headlines;
    this->quality = quality;
    this->truth = truth;
}
