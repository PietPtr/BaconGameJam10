#include "Paper.h"

using namespace sf;

Paper::Paper()
{
}

void Paper::generate()
{
    for (int i = 0; i < 3; i++)
    {
        Image image;
        int width = 180;
        int height = 264 - ((int)(news.headlines[i].text.length() / 22) + 1) * 13;
        std::cout << height << " " << width << " " << news.headlines[i].text << "\n";
        image.create(183, height, Color(0, 0, 0, 0));

        for (int y = 0; y < height; y += 2)
        {
            for (int x = 0; x < width; x++)
            {
                if (!((int)(sin(x * y * height) * 11) % 3 == 0))
                    image.setPixel(x, y, Color(0, 0, 0, 255));
            }
        }
        images.push_back(image);
    }
}

void Paper::draw(Texture* paperTexture, RenderWindow* window)
{
    for (int i = 0; i < 3; i++)
    {
        int lines = (int)(news.headlines[i].text.length() / 22) + 1;

        Vector2f paperPos = Vector2f(210 * i * 2, 20);

        Sprite emptyPaper;
        emptyPaper.setTexture(*paperTexture);
        emptyPaper.setPosition(paperPos);
        emptyPaper.setScale(2, 2);
        window->draw(emptyPaper);

        Sprite letters;
        Texture letterTexture;
        letterTexture.loadFromImage(images[i]);
        letters.setTexture(letterTexture);
        letters.setPosition(paperPos + Vector2f(20, 32 + lines * 13 * 2));
        letters.setScale(2, 2);
        window->draw(letters);

        RectangleShape qualityBar;
        qualityBar.setSize(Vector2f(190 * (news.headlines[i].quality / 100.0) * 2, 10));
        qualityBar.setPosition(paperPos + Vector2f(16, 624));
        qualityBar.setOutlineThickness(2);
        qualityBar.setOutlineColor(Color(100, 100, 100));
        qualityBar.setFillColor(Color(150, 150, 150));
        window->draw(qualityBar);

        RectangleShape truthBar;
        truthBar.setSize(Vector2f(190 * (news.headlines[i].truth / 100.0) * 2, 10));
        truthBar.setPosition(paperPos + Vector2f(16, 664));
        truthBar.setOutlineThickness(2);
        truthBar.setOutlineColor(Color(100, 100, 100));
        truthBar.setFillColor(Color(150, 150, 150));
        window->draw(truthBar);
    }
}
