#include <main_p.h>

int main(int argc, char const *argv[])
{
    Game* game = new Game();
    int i = game->run();
    delete game;
    return i;
}
