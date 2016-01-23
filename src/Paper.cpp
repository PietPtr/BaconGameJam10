#include "Paper.h"

Paper::Paper()
{
    //ctor
}

void Paper::generate(Texture paperTexture)
{
    for (int i = 0; i < 3; i++)
    {
        Image image;
        image.create(183, 264 - (news.headlines[i].text.length() % 22) * 13);
    }
}

void Paper::draw()
{

}
