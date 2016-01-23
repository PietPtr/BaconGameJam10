#ifndef PAPER_H
#define PAPER_H
#include "include.h"
#include <SFML/Graphics.hpp>
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
