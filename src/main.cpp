#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

using namespace sf;

Texture* fontTexture;

int main()
{
    srand(time(NULL));

    int windowWidth = 1280;
    int windowHeigth = 720;
    RenderWindow window(VideoMode(windowWidth, windowHeigth), "Float");
    window.setVerticalSyncEnabled(true);

    Game game(&window);

    game.initialize();

    fontTexture = game.getFontTexture();

    while (window.isOpen())
    {
        game.update();
        game.draw();
    }

    return 0;
}

void drawString(std::string originalText, Vector2f position, Color color, int lineLength, RenderWindow* window)
{
    /*std::string text;
    for (int i = 0; i < (int)(originalText.length() / lineLength); i++)
    {
        //substrLen = lineLength >
        std::string substring = originalText.substr(i * lineLength, lineLength);
        std::cout << substring << "\n";
        for (int l = substring.length() - 1; l >= 0; l--)
        {
            if (substring[l] == ' ')
            {
                substring[l] = '&';
                text += substring;
            }
        }
    }*/
    std::string text = originalText;

    int line = 0;
    for (int i = 0; i < text.length(); i++)
    {
        if (i % lineLength == 0)
            line++;

        Vector2i letterSize = Vector2i(7, 13);

        float letterScale = 2;

        Sprite letter;
        letter.setTexture(*fontTexture);
        letter.setScale(letterScale, letterScale);
        if (int(text[i]) >= 65 && text[i] <= 80)
            letter.setTextureRect(IntRect(((int)text[i] - 65) * letterSize.x, 0 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i]) >= 81 && text[i] <= 90)
            letter.setTextureRect(IntRect(((int)text[i] - 81) * letterSize.x, 1 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i]) >= 97 && text[i] <= 102)
            letter.setTextureRect(IntRect(((int)text[i] - 87) * letterSize.x, 1 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i]) >= 103 && text[i] <= 118)
            letter.setTextureRect(IntRect(((int)text[i] - 103) * letterSize.x, 2 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i]) >= 119 && text[i] <= 122)
            letter.setTextureRect(IntRect(((int)text[i] - 119) * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i]) >= 48 && text[i] <= 54)
            letter.setTextureRect(IntRect(((int)text[i] - 39) * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if ((int)text[i] >= 55 && (int)text[i] <= 57)
            letter.setTextureRect(IntRect(((int)text[i] - 55) * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i] == 46))
            letter.setTextureRect(IntRect(4 * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i] == 44))
            letter.setTextureRect(IntRect(5 * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i] == 34))
            letter.setTextureRect(IntRect(6 * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i] == 33))
            letter.setTextureRect(IntRect(7 * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if (int(text[i] == 63))
            letter.setTextureRect(IntRect(8 * letterSize.x, 3 * letterSize.y, letterSize.x, letterSize.y));
        else if (text[i] == 32)
            letter.setTextureRect(IntRect(4 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        else if (text[i] == 43)
            letter.setTextureRect(IntRect(5 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        else if (text[i] == 45)
            letter.setTextureRect(IntRect(6 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        else if (text[i] == 39)
            letter.setTextureRect(IntRect(7 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        else
            letter.setTextureRect(IntRect(3 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        letter.setPosition(position.x + (i % lineLength) * (letterSize.x + 1) * letterScale, position.y + (line - 1) * letterSize.y * letterScale);
        letter.setColor(color);
        window->draw(letter);

    }
}
