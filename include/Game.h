#include <SFML/Graphics.hpp>
#include <iostream>
#include "Audio.h"
#include "include.h"
#include "Paper.h"

using namespace sf;

enum GameState
{
    INTRO,
    HANK_STATUS,
    HANK_MESSAGE,
    SELECTION,
    REACTION
};

class Game
{
    public:
        Game(RenderWindow* window);
        void initialize();

        void update();
        void updateReaction();
        void recalculateStats();

        void draw();
        void drawIntro();
        void drawHank();
        void drawHankStatus();
        void drawSelection();
        void drawReaction();

        void drawString(std::string, Vector2f position, Color color, int lineLength);
        void loadNews();
        Headline createHeadline(std::string line);
        bool isWindowOpen();
        bool drawHankSpeaking(int startTime, std::string text);

        void loadAudio(std::vector<std::string> fileNames);
        void loadTextures(std::vector<std::string> fileNames);

        int randint(int low, int high, int seed);
        int randint(int low, int high);
    protected:
    private:
        RenderWindow* window;
        GameState gamestate = INTRO;
        GameState lastGamestate = INTRO;

        Time dt;
        Time totalTime;
        Clock clock;
        int frame = 0;

        bool introDone = false;
        int timeHankStartedTalking = 0;
        int lastReactionUpdateTime = 0;
        bool doneSpeaking = false;
        News currentNews;
        Paper currentPaper;

        int sales = 50;
        int influence = 50;
        int sympathy = 50;
        int emotion = 50;
        int money = 50;
        int food = 50;

        int newSympathy = 0;
        int newEmotion = 0;
        int newMoney = 0;
        int newFood = 0;

        int articleChosen = -1;

        std::vector<std::string> audioFileNames { };
        std::vector<std::string> textureFileNames { "font.png", "hank-bg.png", "moustache.png", "table.png", "empty-paper.png", "result-bg.png", "diamond.png" };
        std::vector<Audio*> sfx;

        std::vector<News> startNews;
        std::vector<News> otherNews;

        std::vector<Texture> textures;

        std::string introText = "Good morning sir. Welcome to your new job as editor of the Transmalvanian     Post. I am Hank, I will report on the most recent news stories and show you   the articles the journalists have written. You are the editor, so you decide  what appears in this newspaper. Choose carefully, for the news is a powerful  weapon with many uses. Click anywhere to continue.";
};
