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
        int width = 183;
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
        letters.setPosition(paperPos + Vector2f(16, 32 + lines * 13 * 2));
        letters.setScale(2, 2);
        window->draw(letters);


    }
}
