#ifndef PAPER_H
#define PAPER_H
#include "include.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class Paper
{
    public:
        Paper();
        void generate(Texture paperTexture);
        void draw();
    protected:
    private:
        News news;
        std::vector<Image> images;
};

#endif // PAPER_H
