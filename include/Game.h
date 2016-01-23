#include <SFML/Graphics.hpp>
#include <iostream>
#include "Audio.h"

using namespace sf;

class Game
{
    public:
        Game(RenderWindow* window);
        void initialize();
        void update();
        void draw();
        void drawString(std::string, Vector2f position);
        bool isWindowOpen();

        void loadAudio(std::vector<std::string> fileNames);
        void loadTextures(std::vector<std::string> fileNames);

        int randint(int low, int high, int seed);
        int randint(int low, int high);
    protected:
    private:
        RenderWindow* window;

        Time dt;
        Time totalTime;
        Clock clock;
        int frame = 0;

        std::vector<std::string> audioFileNames { };
        std::vector<std::string> textureFileNames { "font.png", "hank-bg.png", "moustache.png" };
        std::vector<Audio*> sfx;

        std::vector<Texture> textures;
};
