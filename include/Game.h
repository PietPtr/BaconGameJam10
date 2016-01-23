#include <SFML/Graphics.hpp>
#include <iostream>
#include "Audio.h"
#include "include.h"

using namespace sf;

enum GameState
{
    INTRO,
    HANK,
    SELECTION,
    REACTION
};

class Game
{
    public:
        Game(RenderWindow* window);
        void initialize();

        void update();

        void draw();
        void drawIntro();
        void drawHank();
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

        Time dt;
        Time totalTime;
        Clock clock;
        int frame = 0;

        bool introDone = false;
        int timeHankStartedTalking = 0;
        bool doneSpeaking = false;
        News currentNews;

        std::vector<std::string> audioFileNames { };
        std::vector<std::string> textureFileNames { "font.png", "hank-bg.png", "moustache.png", "table.png", "empty-paper.png" };
        std::vector<Audio*> sfx;

        std::vector<News> startNews;
        std::vector<News> otherNews;

        std::vector<Texture> textures;

        std::string introText = "Good morning sir. Welcome to your new job as editor of the Transmalvanian     Post. I am Hank, I will report on the most recent news stories and show you   the articles the journalists have written. You are the editor, so you decide  what appears in this newspaper. Choose carefully, for the news is a powerful  weapon with many uses. Click anywhere to continue.";
};
