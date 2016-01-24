#include "Label.h"

using namespace sf;

void drawString(std::string text, Vector2f position, Color color, int lineLength, RenderWindow* window);

Label::Label(std::string text, Vector2f position, int currentTime)
{
    this->text = text;
    this->position = position;
    this->creationTime = currentTime;
}

void Label::draw(RenderWindow* window, int currentTime)
{
    RectangleShape bg(Vector2f(text.length() * 16, 26));
    bg.setFillColor(Color(50, 50, 50));
    bg.setPosition(position - Vector2f(2, 2));
    bg.setOutlineThickness(2);
    bg.setOutlineColor(Color(25, 25, 25));
    window->draw(bg);

    drawString(text, position, Color(234, 234, 234), 512, window);
}
