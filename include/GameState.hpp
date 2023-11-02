#pragma once

#include <SDL.h>

class GameState
{
public:
    GameState()
    {
        playerObj.x = posx;
        playerObj.y = posy;
        playerObj.w = playerWidth;
        playerObj.h = playerHeight;
    }
    void moveLeft()
    {
        // if not colliding
        posx--;
        if (posx < 1)
            posx = 1;
        playerObj.x = posx;
        tileId = 57;
    }
    void moveRight()
    {
        // if not colliding
        posx++;
        if (posx > 639)
            posx = 639;
        playerObj.y = posy;
        tileId = 53;
    }
    void jump()
    {
        // implement player jump
    }
    void playerAnimation()
    {
    }

    // A level is 20 tiles by 10 tiles (in the viewport/camera)
    // height = 20*16 = 320px
    // width = 10*16 = 160px
private:
    const int playerWidth = 20;
    const int playerHeight = 16;
    const float gravity = 0.5f;

    int posx = 2;
    int posy = 2;
    int lives = 3;
    int score = 0;
    int currentLevel;
    int viewX = 0;
    int viewY = 0;
    int tileId = 0;
    int tileOffset = 0;

    bool gotTrophy = false;
    bool canClimb = false;
    SDL_Rect playerObj;
};