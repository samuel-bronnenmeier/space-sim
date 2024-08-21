#include <iostream>
#include <string>
#include <random>
#include <ctime>

#include <Stats_p.h>
#include <Control_p.h>
#include <Error_p.h>
#include <Map_p.h>
#include <Help_p.h>

class Game
{
private:
    // Stores the turn, the player is currently in
    long unsigned int turn;

    // Stores the player's name
    std::string playerName;

    // An object containing the number of ressources, ...
    Stats stats;

    // The map of the planet
    Map *map;

    // Flag for quitting
    bool quit;

    // Flag for death
    bool dead;

public:
    Game();

    int run();

    void displayIntro();

    int getInput();

    void handleInput(int input);

    // These methods handle the different things the player can do
    void explore();
    void build();
    void extract();

    void updateStats();

    void displayStats(bool endOfTurn);

    void displayOutro();

    int errorHandler(int e);

    int calculateActualPrice(int price);

    void kill(int peopleToKill);

    void die();

    ~Game();
};