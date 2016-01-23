#ifndef PAPER_H
#define PAPER_H
#include "include.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class Paper
{
    public:
        Paper();
        void setNews(News news) { this->news = news; };
        void generate();
        void draw(Texture* paperTexture, RenderWindow* window);
    protected:
    private:
        News news;
        std::vector<Image> images;
};

#endif // PAPER_H
