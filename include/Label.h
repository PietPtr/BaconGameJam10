#ifndef LABEL_H
#define LABEL_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Label
{
    public:
        Label(std::string text, Vector2f position, int currentTime);
        void draw(RenderWindow* window, int currentTime);
        bool isGone(int currentTime) { return currentTime - creationTime > 2500; }
    protected:
    private:
        std::string text;
        Vector2f position;
        int creationTime;
};

#endif // LABEL_H
