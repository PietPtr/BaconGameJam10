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
        if (event.type == sf::Event::MouseButtonPressed)
            {
                if (gamestate == INTRO && introDone)
                {
                    gamestate = HANK;
                    currentNews = startNews.at(randint(0, startNews.size() - 1));
                    timeHankStartedTalking = totalTime.asMilliseconds();
                }
                if (gamestate == HANK && doneSpeaking)
                {
                    gamestate = SELECTION;
                }
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
    }

    dt = clock.restart();
    totalTime += dt;

    switch (gamestate)
    {
    case INTRO:

        break;
    case HANK:

        break;
    case SELECTION:

        break;
    case REACTION:

        break;
    }

    frame++;
}

void Game::draw()
{
    window->clear();

    //drawString("King wastes millions   on failing parade.", Vector2f(0, 0));

    switch (gamestate)
    {
    case INTRO:
        drawIntro();
        break;
    case HANK:
        drawHank();
        break;
    case SELECTION:
        //drawSelection();
        break;
    case REACTION:
        //drawReaction();
        break;
    }

    window->display();
}

void Game::drawIntro()
{
    Sprite bg;
    bg.setTexture(textures[1]);
    bg.setScale(2, 2);
    window->draw(bg);

    Sprite moustache;
    moustache.setTexture(textures[2]);
    moustache.setScale(4, 4);
    moustache.setOrigin(22, 7);
    moustache.setPosition(Vector2f(265, 437 + sin(frame / 5) * 6));
    moustache.setRotation(sin(frame / 8) * 2);
    window->draw(moustache);

    introDone = drawHankSpeaking(0, introText);
}

void Game::drawHank()
{
    Sprite bg;
    bg.setTexture(textures[1]);
    bg.setScale(2, 2);
    window->draw(bg);

    Sprite moustache;
    moustache.setTexture(textures[2]);
    moustache.setScale(4, 4);
    moustache.setOrigin(22, 7);
    moustache.setPosition(Vector2f(265, 437 + sin(frame / 5) * 6));
    moustache.setRotation(sin(frame / 8) * 2);
    window->draw(moustache);

    doneSpeaking = drawHankSpeaking(timeHankStartedTalking, currentNews.message);
}

bool Game::drawHankSpeaking(int startTime, std::string text)
{
    std::string textToBeDrawn;
    int delay = 25;
    int limit = (totalTime.asMilliseconds() - startTime) / delay > text.length() ? text.length() : (totalTime.asMilliseconds() - startTime) / delay;
    for (int i = 0; i < limit; i++)
    {
        textToBeDrawn += text[i];
    }
    drawString(textToBeDrawn, Vector2f(4, 520), Color(200, 200, 200), 78);

    return limit == text.length();
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
    std::ifstream newsFile("assets/news");
    if (newsFile.is_open())
    {
        News news;
        Headline headlines[3];

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
                news.message = fileContent;
                news.message.erase(news.message.begin());
                std::cout << news.message << "\n";
            }

            if (fileContent[0] == 's')
            {
                std::string str_sympathy = fileContent;
                str_sympathy.erase(str_sympathy.begin());
                news.sympathy = std::stoi(str_sympathy, nullptr);
            }
            if (fileContent[0] == 'e')
            {
                std::string str_emotion = fileContent;
                str_emotion.erase(str_emotion.begin());
                news.emotion = std::stoi(str_emotion, nullptr);
            }
            if (fileContent[0] == 'm')
            {
                std::string str_money = fileContent;
                str_money.erase(str_money.begin());
                news.money = std::stoi(str_money, nullptr);
            }
            if (fileContent[0] == 'f')
            {
                std::string str_food = fileContent;
                str_food.erase(str_food.begin());
                news.food = std::stoi(str_food, nullptr);
            }
            if (fileContent[0] == '-')
            {
                news.headlines[0] = headlines[0];
                news.headlines[1] = headlines[1];
                news.headlines[2] = headlines[2];

                if (news.food == 0 && news.sympathy == 0 && news.emotion == 0 && news.money == 0)
                    startNews.push_back(news);
                else
                    otherNews.push_back(news);
            }

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
    std::string str_sympathy;
    std::string str_emotion;
    std::string str_money;
    std::string str_food;
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
        else if (reading == 3)
            str_sympathy += line[i];
        else if (reading == 4)
            str_emotion += line[i];
        else if (reading == 5)
            str_money += line[i];
        else if (reading == 6)
            str_food += line[i];
    }

    //std::cout << "\"" << text << "\" quality: " << str_quality << ", truth: " << str_truth << "\n";
    headline.text = text;
    std::string::size_type sz;
    headline.quality = std::stoi(str_quality, &sz);
    headline.truth = std::stoi(str_truth, &sz);
    headline.sympathy = std::stoi(str_sympathy, &sz);
    headline.emotion = std::stoi(str_emotion, &sz);
    headline.money = std::stoi(str_money, &sz);
    headline.food = std::stoi(str_food, &sz);

    return headline;
}

void Game::drawString(std::string text, Vector2f position, Color color, int lineLength)
{
    int line = 0;
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
        letter.setColor(color);
        window->draw(letter);

    }
}

int Game::randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    srand(totalTime.asMicroseconds() * value * rand());

    return value;
}
