#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Game.h"

using namespace sf;

void drawString(std::string text, Vector2f position, Color color, int lineLength, RenderWindow* window);

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
                    gamestate = HANK_MESSAGE;
                    currentNews = startNews.at(0);//randint(0, startNews.size() - 1));
                    timeHankStartedTalking = totalTime.asMilliseconds();
                }
                else if (gamestate == HANK_MESSAGE)// && doneSpeaking)
                {
                    gamestate = SELECTION;

                    if (finishedGame != UNFINISHED)
                        gamestate = HANK_GAMEDONE;

                    std::cout << currentNews.endMessage << "\n";

                    timeHankStartedTalking = totalTime.asMilliseconds();

                    currentPaper.setNews(currentNews);
                    currentPaper.generate();
                }
                else if (gamestate == HANK_STATUS)
                {
                    gamestate = HANK_MESSAGE;
                    timeHankStartedTalking = totalTime.asMilliseconds();
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
                    gamestate = HANK_STATUS;

                    int lowestDistanceIndex = 0;
                    int lowestDistance = 10000;
                    for (int i = 0; i < otherNews.size(); i++)
                    {
                        int distance = 0;
                        distance += otherNews[i].sympathy == -1 ? 10 : abs(sympathy - otherNews[i].sympathy);
                        distance += otherNews[i].emotion == -1 ? 10 : abs(emotion - otherNews[i].emotion);
                        distance += otherNews[i].money == -1 ? 10 : abs(money - otherNews[i].money);
                        distance += otherNews[i].food == -1 ? 10 : abs(food - otherNews[i].food);
                        if (distance < lowestDistance)
                        {
                            lowestDistanceIndex = i;
                            lowestDistance = distance;
                        }
                        std::cout << distance << " distance at " << i << "\n";
                    }
                    //std::cout << "lowest distance: " << lowestDistance << ", " << lowestDistanceIndex << "\n";
                    currentNews = otherNews[lowestDistanceIndex];

                    otherNews.erase(otherNews.begin()+lowestDistanceIndex);

                    timeHankStartedTalking = totalTime.asMilliseconds();

                    std::cout << currentNews.endMessage.length() << "<- endmessage \n";

                    if (lowestDistance > 50)
                    {
                        finishedGame = BALANCED;
                        timeHankStartedTalking = totalTime.asMilliseconds();
                    }
                    if (currentNews.endMessage.length() != 0)
                    {
                        std::cout << "The finishedgame var was set to DEATH in line 114, endmsg:" << currentNews.endMessage.length() <<"\n";
                        finishedGame = DEATH;
                        timeHankStartedTalking = totalTime.asMilliseconds();
                    }


                    std::vector<std::string> emptyStrList;
                    labelTexts = emptyStrList;
                    std::vector<Label> emptyLabelList;
                    labels = emptyLabelList;

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
    case HANK_MESSAGE:

        break;
    case SELECTION:

        break;
    case REACTION:
        updateReaction();
        break;
    }

    lastGamestate = gamestate;
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
}

void Game::recalculateStats()
{
    Headline chosenHeadline = currentNews.headlines[articleChosen];
    float salesMod = pow(2, chosenHeadline.quality / 50.0 - 1);
    sales *= salesMod;

    float influenceMod = pow(2, chosenHeadline.truth / 50.0 - 1);
    influence *= influenceMod;

    sales = sales > 100 ? 100 : sales;
    influence = influence > 100 ? 100 : influence;
    sales = sales < 0 ? 0 : sales;
    influence = influence < 0 ? 0 : influence;

    float statsMod = (sales / 300.0) + (influence / 100.0);

    std::cout << "influence: " << influence << ", sales: " << sales << ", statsMod: " << statsMod << "\n";

    newSympathy = (int)(sympathy + chosenHeadline.sympathy * statsMod);
    newEmotion = (int)(emotion + chosenHeadline.emotion * statsMod);
    newMoney = (int)(money + chosenHeadline.money * statsMod);
    newFood = (int)(food + chosenHeadline.food * statsMod);

    newSympathy = newSympathy > 100 ? 100 : newSympathy;
    newSympathy = newSympathy < 0 ? 0 : newSympathy;
    newEmotion = newEmotion > 100 ? 100 : newEmotion;
    newEmotion = newEmotion < 0 ? 0 : newEmotion;
    newMoney = newMoney > 100 ? 100 : newMoney;
    newMoney = newMoney < 0 ? 0 : newMoney;
    newFood = newFood > 100 ? 100 : newFood;
    newFood = newFood < 0 ? 0 : newFood;

    if (newEmotion > emotion) //people got angry
        labelTexts.insert(std::end(labelTexts), std::begin(react_angry), std::end(react_angry));
    if (newEmotion < emotion)
        labelTexts.insert(std::end(labelTexts), std::begin(react_happy), std::end(react_happy));
    if (newSympathy > sympathy)
        labelTexts.insert(std::end(labelTexts), std::begin(react_militairy), std::end(react_militairy));
    if (newSympathy < sympathy)
        labelTexts.insert(std::end(labelTexts), std::begin(react_monarch), std::end(react_monarch));
    if (chosenHeadline.truth < 30)
        labelTexts.insert(std::end(labelTexts), std::begin(react_lowTruth), std::end(react_lowTruth));
    if (chosenHeadline.truth > 70)
        labelTexts.insert(std::end(labelTexts), std::begin(react_highTruth), std::end(react_highTruth));
    if (chosenHeadline.quality < 30)
        labelTexts.insert(std::end(labelTexts), std::begin(react_lowQuality), std::end(react_lowQuality));
    if (chosenHeadline.quality > 70)
        labelTexts.insert(std::end(labelTexts), std::begin(react_highQuality), std::end(react_highQuality));

    for (int x = 0; x < 2; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            Label label(labelTexts[randint(0, labelTexts.size() - 1)],
                    Vector2f(350 + x * 512, 50 + y * 384 ), totalTime.asMilliseconds());
            labels.push_back(label);
        }
    }
}

void Game::draw()
{
    window->clear();

    switch (gamestate)
    {
    case INTRO:
        drawIntro();
        break;
    case HANK_MESSAGE:
        drawHank();
        break;
    case HANK_STATUS:
        drawHankStatus();
        break;
    case HANK_GAMEDONE:
        drawHankGamedone();
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

void Game::drawHankStatus()
{
    std::string statusMessage;

    if (sales > 50)
        statusMessage += "It looks like the Transmalvanian Post is selling very well, good job! ";
    else
        statusMessage += "The Transmalvanian Post does not seem to be doing great. Perhaps you should select articles that are more popular? ";

    if (influence > 70)
        statusMessage += "People hold our newspaper in high esteem, they seem to be influenced greatly by our opinions. Do not abuse this power.";
    else if (influence < 30)
        statusMessage += "People do not seem to take us seriously. I personally think our articles should stick to the truth more.";

    if (statusMessage.length() == 0)
        statusMessage += "Not much has changed as far as our sales and general reputation go.";

    Sprite bg;
    bg.setTexture(textures[1]);
    bg.setScale(2, 2);
    window->draw(bg);

    doneSpeaking = drawHankSpeaking(timeHankStartedTalking, statusMessage);

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

void Game::drawHankGamedone()
{
    Sprite bg;
    bg.setTexture(textures[1]);
    bg.setScale(2, 2);
    window->draw(bg);

    bool isDoneSpeaking = true;
    if (finishedGame == BALANCED)
        isDoneSpeaking = drawHankSpeaking(timeHankStartedTalking, "Congratualations, you have succesfully completed your term as an editor of the Transmalvanian Post. The King and the General are still alive, under your command our newspaper has informed the people.");
    else if (finishedGame == DEATH)
        isDoneSpeaking = drawHankSpeaking(timeHankStartedTalking, currentNews.endMessage);


    Sprite moustache;
    moustache.setTexture(textures[2]);
    moustache.setScale(4, 4);
    moustache.setOrigin(22, 7);

    if (!isDoneSpeaking)
    {
        moustache.setPosition(Vector2f(265, 437 + sin(frame / 5) * 6));
        moustache.setRotation(sin(frame / 8) * 2);
    }
    else
    {
        moustache.setPosition(Vector2f(265, 437));
    }
    window->draw(moustache);

    std::cout << "Game is finished because reason " << finishedGame << ".\n";
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
        drawString(currentNews.headlines[i].text, Vector2f(210 * i * 2 + 20, 42), Color(0, 0, 0, 255), 22, window);
        drawString("Popularity", Vector2f(104 + 210 * i * 2, 614), Color(150, 150, 150), 22, window);
        drawString("Truth", Vector2f(112 + 210 * i * 2, 658), Color(150, 150, 150), 22, window);
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

    drawString("Sympathy", Vector2f(88, 54), Color(150, 150, 150), 22, window);
    drawString("Emotion", Vector2f(96, 192), Color(150, 150, 150), 22, window);
    drawString("Money", Vector2f(108, 330), Color(150, 150, 150), 22, window);
    drawString("Food", Vector2f(116, 468), Color(150, 150, 150), 22, window);

    drawString("Click to advance to the next day.", Vector2f(752, 690), Color(25, 25, 25), 100, window);

    for (int i = 0; i < labels.size(); i++)
    {
        labels[i].draw(window, totalTime.asMilliseconds());
    }
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
    drawString(textToBeDrawn, Vector2f(4, 520), Color(200, 200, 200), 78, window);

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
            if (fileContent[0] == '>')
            {
                news.endMessage = fileContent;
                news.endMessage.erase(news.endMessage.begin());
            }
            if (fileContent[0] == '-')
            {
                news.headlines[0] = headlines[0];
                news.headlines[1] = headlines[1];
                news.headlines[2] = headlines[2];

                if (news.food == -1 && news.sympathy == -1 && news.emotion == -1 && news.money == -1)
                    startNews.push_back(news);
                else
                    otherNews.push_back(news);

                for (int i = 0; i < 3; i++)
                {
                    Headline newHeadline;
                    headlines[i] = newHeadline;
                }
                News newNews;
                news = newNews;
            }

        }
        newsFile.close();
    }
    for (int i = 0; i < otherNews.size(); i++)
    {
        std::cout << otherNews[i].message << ", " << otherNews[i].endMessage.length() << "\n";
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

int Game::randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    srand(totalTime.asMilliseconds() * value * rand());

    return value;
}
