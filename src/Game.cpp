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
                if (gamestate == INTRO)//) && introDone)
                {
                    gamestate = HANK;
                    currentNews = startNews.at(randint(0, startNews.size() - 1));

                    timeHankStartedTalking = totalTime.asMilliseconds();
                }
                else if (gamestate == HANK)// && doneSpeaking)
                {
                    gamestate = SELECTION;
                    currentPaper.setNews(currentNews);
                    currentPaper.generate();
                }
                else if (gamestate == SELECTION)
                {
                    gamestate = REACTION;
                    lastReactionUpdateTime = totalTime.asMilliseconds();

                    if (Mouse::getPosition(*window).x < 1280 / 3)
                        articleChosen = 0;
                    else if (Mouse::getPosition(*window).x > 1280 / 3 && Mouse::getPosition(*window).x < 2 * 1280 / 3)
                        articleChosen = 1;
                    else if (Mouse::getPosition().x > 2 * 1280 / 3)
                        articleChosen = 2;

                    recalculateStats();
                }
                else if (gamestate == REACTION && sympathy == newSympathy && emotion == newEmotion && money == newMoney && food == newFood)
                {
                    gamestate = HANK;
                }
                //std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                //std::cout << "mouse y: " << event.mouseButton.y << std::endl;
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
        updateReaction();
        break;
    }

    frame++;
}

void Game::updateReaction()
{
    if (totalTime.asMilliseconds() - lastReactionUpdateTime > 250)
    {

        if (sympathy != newSympathy)
            sympathy += (newSympathy > sympathy ? 1 : -1);
        if (emotion != newEmotion)
            emotion += (newEmotion > emotion ? 1 : -1);
        if (money != newMoney)
            money += (newMoney > money ? 1 : -1);
        if (food != newFood)
            food += (newFood > food ? 1 : -1);
        lastReactionUpdateTime = totalTime.asMilliseconds();
    }
    std::cout << sympathy << "\n";
}

void Game::recalculateStats()
{
    Headline chosenHeadline = currentNews.headlines[articleChosen];
    float salesMod = pow(2, chosenHeadline.quality / 50.0 - 1);
    sales *= salesMod;

    float influenceMod = pow(2, chosenHeadline.truth / 50.0 - 1);
    influence *= influenceMod;

    newSympathy = sympathy + chosenHeadline.sympathy;
    newEmotion = emotion + chosenHeadline.emotion;
    newMoney = money + chosenHeadline.money;
    newFood = food + chosenHeadline.food;

    newSympathy = newSympathy > 100 ? 100 : newSympathy;
    newSympathy = newSympathy < 0 ? 0 : newSympathy;
    newEmotion = newEmotion > 100 ? 100 : newEmotion;
    newEmotion = newEmotion < 0 ? 0 : newEmotion;
    newMoney = newMoney > 100 ? 100 : newMoney;
    newMoney = newMoney < 0 ? 0 : newMoney;
    newFood = newFood > 100 ? 100 : newFood;
    newFood = newFood < 0 ? 0 : newFood;
}

void Game::draw()
{
    window->clear();

    switch (gamestate)
    {
    case INTRO:
        drawIntro();
        break;
    case HANK:
        drawHank();
        break;
    case SELECTION:
        drawSelection();
        break;
    case REACTION:
        drawReaction();
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

    introDone = drawHankSpeaking(0, introText);

    Sprite moustache;
    moustache.setTexture(textures[2]);
    moustache.setScale(4, 4);
    moustache.setOrigin(22, 7);

    if (!introDone)
    {
        moustache.setPosition(Vector2f(265, 437 + sin(frame / 5) * 6));
        moustache.setRotation(sin(frame / 8) * 2);
    }
    else
    {
        moustache.setPosition(Vector2f(265, 437));
    }
    window->draw(moustache);

}

void Game::drawHank()
{
    Sprite bg;
    bg.setTexture(textures[1]);
    bg.setScale(2, 2);
    window->draw(bg);

    doneSpeaking = drawHankSpeaking(timeHankStartedTalking, currentNews.message);

    Sprite moustache;
    moustache.setTexture(textures[2]);
    moustache.setScale(4, 4);
    moustache.setOrigin(22, 7);

    if (!doneSpeaking)
    {
        moustache.setPosition(Vector2f(265, 437 + sin(frame / 5) * 6));
        moustache.setRotation(sin(frame / 8) * 2);
    }
    else
    {
        moustache.setPosition(Vector2f(265, 437));
    }
    window->draw(moustache);

}

void Game::drawSelection()
{
    Sprite bg;
    bg.setTexture(textures[3]);
    bg.setScale(2, 2);
    window->draw(bg);

    currentPaper.draw(&textures[4], window);

    RectangleShape selector;
    selector.setSize(Vector2f(204 * 2, 296 * 2));
    if (Mouse::getPosition(*window).x < 1280 / 3)
        selector.setPosition(Vector2f(210 * 0 * 2, 20));
    else if (Mouse::getPosition(*window).x > 1280 / 3 && Mouse::getPosition(*window).x < 2 * 1280 / 3)
        selector.setPosition(Vector2f(210 * 1 * 2, 20));
    else if (Mouse::getPosition().x > 2 * 1280 / 3)
        selector.setPosition(Vector2f(210 * 2 * 2, 20));
    selector.setOutlineThickness(2);
    selector.setOutlineColor(Color(255, 255, 255));
    selector.setFillColor(Color(0, 0, 0, 0));
    window->draw(selector);


    for (int i = 0; i < 3; i++)
    {
        drawString(currentNews.headlines[i].text, Vector2f(210 * i * 2 + 20, 42), Color(0, 0, 0, 255), 22);
        drawString("Popularity", Vector2f(104 + 210 * i * 2, 614), Color(150, 150, 150), 22);
        drawString("Truth", Vector2f(112 + 210 * i * 2, 658), Color(150, 150, 150), 22);
    }
}

void Game::drawReaction()
{
    Sprite bg;
    bg.setTexture(textures[5]);
    bg.setScale(2, 2);
    window->draw(bg);

    Sprite sympathyMeter(textures[6]);
    sympathyMeter.setOrigin(Vector2f(3, 3));
    sympathyMeter.setPosition(Vector2f(15 * 2, 47 * 2) + Vector2f(sympathy / 100.0 * 120 * 2, 0));
    sympathyMeter.setScale(2, 2);
    window->draw(sympathyMeter);

    Sprite emotionMeter(textures[6]);
    emotionMeter.setOrigin(Vector2f(3, 3));
    emotionMeter.setPosition(Vector2f(15 * 2, 116 * 2) + Vector2f(emotion / 100.0 * 120 * 2, 0));
    emotionMeter.setScale(2, 2);
    window->draw(emotionMeter);

    Sprite moneyMeter(textures[6]);
    moneyMeter.setOrigin(Vector2f(3, 3));
    moneyMeter.setPosition(Vector2f(15 * 2, 185 * 2) + Vector2f(money / 100.0 * 120 * 2, 0));
    moneyMeter.setScale(2, 2);
    window->draw(moneyMeter);

    Sprite foodMeter(textures[6]);
    foodMeter.setOrigin(Vector2f(3, 3));
    foodMeter.setPosition(Vector2f(15 * 2, 254 * 2) + Vector2f(food / 100.0 * 120 * 2, 0));
    foodMeter.setScale(2, 2);
    window->draw(foodMeter);

    std::string sympathyStr = "Sympathy";
  //  if (sympathy != newSympathy)
    //    sympathyStr = newSympathy > sympathy ? "Sympathy ++" : "Sympathy --";

    std::string emotionStr = "Emotion";
   // if (emotion != newEmotion)
      //  emotionStr = newEmotion > emotion ? "Emotion ++" : "Emotion --";

    std::string moneyStr = "Money";
   // if (money != newMoney)
      //  moneyStr = newMoney > money ? "Money ++" : "Money --";

    std::string foodStr = "Food";
    //if (food != newFood)
       // foodStr = newFood > food ? "Food ++" : "Food --";


    drawString(sympathyStr, Vector2f(88, 54), Color(150, 150, 150), 22);
    drawString(emotionStr, Vector2f(96, 192), Color(150, 150, 150), 22);
    drawString(moneyStr, Vector2f(108, 330), Color(150, 150, 150), 22);
    drawString(foodStr, Vector2f(116, 468), Color(150, 150, 150), 22);

    drawString("Click to advance to the next day.", Vector2f(752, 690), Color(25, 25, 25), 100);
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
                //std::cout << news.message << "\n";
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

                for (int i = 0; i < 3; i++)
                {
                    Headline newHeadline;
                    headlines[i] = newHeadline;
                }
            }

        }
        newsFile.close();
    }
    for (int i = 0; i < startNews.size(); i++)
    {
        std::cout << startNews[i].headlines[0].text << "\n";
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
        else if (text[i] == 43)
            letter.setTextureRect(IntRect(5 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
        else if (text[i] == 45)
            letter.setTextureRect(IntRect(6 * letterSize.x, 4 * letterSize.y, letterSize.x, letterSize.y));
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
    srand(totalTime.asMilliseconds() * value * rand());

    return value;
}
