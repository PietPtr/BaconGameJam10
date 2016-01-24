#include <SFML/Graphics.hpp>
#include <iostream>
#include "Audio.h"
#include "include.h"
#include "Paper.h"
#include "Label.h"

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

        void loadNews();
        Headline createHeadline(std::string line);
        bool isWindowOpen();
        bool drawHankSpeaking(int startTime, std::string text);

        void loadAudio(std::vector<std::string> fileNames);
        void loadTextures(std::vector<std::string> fileNames);
        Texture* getFontTexture() { return &(textures[0]); };

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

        std::vector<Label> labels;
        std::vector<std::string> labelTexts;
        std::vector<std::string> react_lowTruth { "That can't be correct...", "Hmmmm", "I don't think that's true." };
        std::vector<std::string> react_highTruth { "Exactly!", "I agree", "Yes!" };
        std::vector<std::string> react_lowQuality { "Ugh, moneygrabbers.", "You call that news?", "This paper used to be better" };
        std::vector<std::string> react_highQuality { "Formidable piecie of writing", "This paper has improved a lot lately" };
        std::vector<std::string> react_angry { "What!", "I am outraged!", "This can't be true!" };
        std::vector<std::string> react_happy { "Yeah!", "That is amazing news!", "Great" };
        std::vector<std::string> react_militairy { "Hurray for the general!", "I should enlist" };
        std::vector<std::string> react_monarch { "Long live the King!" };

        std::string introText = "Good morning. Welcome to your new job as editor of the Transmalvanian Post.   Transmalvania is a relativily poor country where the monarch, King Cyrilio,   and the head of the army, General Anzhel, are always fighting for the hearts  of the people. This newspaper needs an objective editor to make sure we       publish truthful stories. We try to inform the people, not influence them. I  am Hank, I will show you the articles the journalists have written and presentyou the facts. Then, you select what appears in the paper. Click to continue.";
};
