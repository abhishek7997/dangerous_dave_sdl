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
    }
    void moveRight()
    {
        // if not colliding
        posx++;
        if (posx > 639)
            posx = 639;
        playerObj.y = posy;
    }
    void jump()
    {
        // implement player jump
    }

private:
    const int playerWidth = 20;
    const int playerHeight = 16;
    const float gravity = 0.5f;

    int posx = 2;
    int posy = 2;
    int lives = 3;
    int score = 0;
    int currentLevel;
    int view_x = 0;
    int view_y = 0;

    bool gotTrophy = false;
    bool canClimb = false;
    SDL_Rect playerObj;
};