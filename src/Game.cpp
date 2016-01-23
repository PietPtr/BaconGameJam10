#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Game.h"

using namespace sf;

Game::Game(RenderWindow* _window)
{
    window = _window;
}

void Game::initialize()
{
    loadAudio(audioFileNames);
    loadTextures(textureFileNames);
    loadNews();
}

void Game::update()
{
    Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
            {
                window->close();
            }
        }
    }

    dt = clock.restart();
    totalTime += dt;

    frame++;
}

void Game::draw()
{
    window->clear();

    drawString("King wastes millions   on failing parade.", Vector2f(0, 0));

    window->display();
}

bool Game::isWindowOpen()
{
    return window->isOpen();
}

void Game::loadAudio(std::vector<std::string> audioFileNames)
{
    for (int i = 0; i < audioFileNames.size(); i++)
    {
        sfx.push_back(new Audio());
        sfx.back()->init(audioFileNames[i]);
    }
}

void Game::loadTextures(std::vector<std::string> textureFileNames)
{
    for (int i = 0; i < textureFileNames.size(); i++)
    {
        Texture texture;
        if (!texture.loadFromFile("assets/textures/" + textureFileNames.at(i)))
            window->close();
        textures.push_back(texture);
    }
}

void Game::loadNews()
{
    std::string fileContent;
    std::ifstream newsFile("assets/news/start");
    //newsFile.open("assets/news/start");
    if (newsFile.is_open())
    {
        News news;
        Headline headlines[3];
        std::string message;

        while(getline(newsFile, fileContent))
        {
            if (fileContent[0] == ':')
            {
                for (int i = 0; i < 3; i++)
                {
                    if (headlines[i].quality == -1) // means that the headline is still empty
                    {
                        headlines[i] = createHeadline(fileContent);
                        break;

                    }
                }
            }

            if (fileContent[0] == ';')
            {
                message = fileContent;
                message.erase(message.begin());
                std::cout << message << "\n";
            }

            //if (fileContent[0] == '-')

        }
        newsFile.close();
    }
}

Headline Game::createHeadline(std::string line)
{
    Headline headline;
    int reading = 0;
    line.erase(line.begin());
    std::string text;
    std::string str_quality;
    std::string str_truth;
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ':')
        {
            reading++;
            continue;
        }
        if (reading == 0)
            text += line[i];
        else if (reading == 1)
            str_quality += line[i];
        else if (reading == 2)
            str_truth += line[i];
    }

    std::cout << "\"" << text << "\" quality: " << str_quality << ", truth: " << str_truth << "\n";
    headline.text = text;
    std::string::size_type sz;
    headline.quality = std::stoi(str_quality, &sz);
    headline.truth = std::stoi(str_truth, &sz);

    return headline;
}

void Game::drawString(std::string text, Vector2f position)
{
    int line = 0;
    int lineLength = 22;
    for (int i = 0; i < text.length(); i++)
    {
        if (i % lineLength == 0)
            line++;

        Vector2i letterSize = Vector2i(7, 13);

        float letterScale = 2;

        Sprite letter;
        letter.setTexture(textures[0]);
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
        else
            letter.setTextureRect(IntRect(3 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        letter.setPosition(position.x + (i % lineLength) * (letterSize.x + 1) * letterScale, position.y + (line - 1) * letterSize.y * letterScale);
        window->draw(letter);

    }
}

int Game::randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    srand(totalTime.asMicroseconds() * value * rand());

    return value;
}
