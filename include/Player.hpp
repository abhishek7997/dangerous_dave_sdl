class Player
{
public:
    void moveLeft()
    {
        // if not colliding
        posx--;
        if (posx < 1)
            posx = 1;
    }
    void moveRight()
    {
        // if not colliding
        posx++;
        if (posx > 639)
            posx = 639;
    }

private:
    int posx = 0;
    int posy = 0;
};