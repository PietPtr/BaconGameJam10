#ifndef NEWS_H
#define NEWS_H
#include <iostream>

class News
{
    public:
        News();
        void initiate(std::string message, std::string* headlines, int quality, int truth);
    protected:
    private:
        std::string message;
        std::string* headlines;
        int quality;
        int truth;
};

#endif // NEWS_H
