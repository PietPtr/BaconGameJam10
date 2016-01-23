#ifndef PAPER_H
#define PAPER_H

#include <SFML/Graphics.hpp>
#include "News.h"
using namespace sf;

class Paper
{
    public:
        Paper();
        void generate();
        void draw();
    protected:
    private:
        News news;
        Image image;
};

#endif // PAPER_H
