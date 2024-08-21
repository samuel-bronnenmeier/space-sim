#include <Game_p.h>

Game::Game()
{
    turn = 1;

    stats.people = STARTING_PEOPLE;
    stats.food = STARTING_FOOD;
    stats.water = STARTING_WATER;
    stats.stone = 0;

    stats.peopleKilled = 0;

    map = new Map;

    quit = false;
}

int Game::run()
{
    displayIntro();

    int input;

    while (!quit)
    {
        input = getInput();

        handleInput(input);

        updateStats();

        displayStats(true);

        turn++;
    }

    displayOutro();

    return 0;
}

void Game::displayIntro()
{
    std::cout << "Welcome to Space Colony Simulator!" << std::endl;
    std::cout << "Type in your name: ";
    std::cin >> playerName;
    std::cout << "Hello " << playerName << "!" << std::endl;
}

int Game::getInput()
{
    std::cout << "What do you want to do? (0=quit, 1=explore, 2=build, 3=extract, 4=stats, 5=help)" << std::endl;
    int input = -1;
    std::cin >> input;
    return input;
}

void Game::handleInput(int input)
{
    switch (input)
    {
    case CONTROL_ERROR:
        errorHandler(ERROR_INPUT_HANDLING);
        break;

    case CONTROL_QUIT:
        quit = true;
        break;

    case CONTROL_EXPLORE:
        explore();
        break;

    case CONTROL_BUILD:
        break;

    case CONTROL_EXTRACT:
        break;

    case CONTROL_STATS:
        displayStats(false);
        break;

    case CONTROL_HELP:
        break;

    default:
        errorHandler(ERROR_INPUT_HANDLING);
        break;
    }
}

void Game::explore()
{
    std::cout << "You decided to go exploring unknown parts of the planet." << std::endl;
    std::cout << "What direction do you want to go? (0=north, 1=east, 2=south, 3=west)" << std::endl;

    int dir = 0;
    std::cin >> dir;

    int price = map->getMaxPrice(dir);
    std::cout << price << " people could be killed, if you go there. Do you want to do it? (no=0, yes=1)" << std::endl;

    int confirm = 0;
    std::cin >> confirm;

    if (confirm)
    {
        int tile = map->exploreTile(dir);
        int killed = calculateActualPrice(price);
        stats.peopleKilled += killed;
        std::cout << killed << " people died during their exploration mission. ";

        if (stats.people > killed)
        {
            stats.people -= killed;

            switch (tile)
            {
            case TILE_EMPTY:
                std::cout << "The remaining part of the group comes back empty-handed..." << std::endl;
                break;

            case TILE_WATER:
                std::cout << "The remaining part brought back some water! You get " << price << " units of water." << std::endl;
                stats.water += price;
                break;

            case TILE_STONE:
                std::cout << "The remaining part of the group managed to bring back " << price << " stones." << std::endl;
                stats.stone += price;
                break;

            default:
                break;
            }
        }
        else
        {
            stats.peopleKilled -= stats.peopleKilled - stats.people;
            stats.people = 0;
        }
    }
    else
    {
        std::cout << "You decide to not go exploring..." << std::endl;
        int input = getInput();
        handleInput(input);
    }
}

void Game::updateStats()
{
    if (stats.people == 0)
    {
        die();
    }
    else
    {
        std::cout << "Your colony is drinking and eating..." << std::endl;

        int foodNeeded = stats.people / 2;
        int waterNeeded = stats.people;

        if (stats.food < foodNeeded)
        {
            std::cout << "Some people didn't get something to eat. They died." << std::endl;
            kill(foodNeeded - stats.food);
        }
        else
        {
            stats.food -= foodNeeded;
        }

        if (stats.water < waterNeeded)
        {
            std::cout << "Some people didn't get something to drink. They died." << std::endl;
            kill(waterNeeded - stats.water);
        }
        else
        {
            stats.water -= waterNeeded;
        }

        stats.people += stats.people / 5;
        std::cout << "Your colony grew a bit as new people were made..." << std::endl;
    }
}

void Game::displayStats(bool endOfTurn)
{
    if (endOfTurn)
    {
        std::cout << "At the end of turn " << turn << " your stats are as following:" << std::endl;
    }
    else
    {
        std::cout << "You are currently in turn " << turn << "." << std::endl;
    }

    std::cout << "People: " << stats.people << std::endl;
    std::cout << "Food: " << stats.food << std::endl;
    std::cout << "Water: " << stats.water << std::endl;
    std::cout << "Stone: " << stats.stone << std::endl;

    std::cout << "You killed " << stats.peopleKilled << " people so far." << std::endl;
}

void Game::displayOutro()
{
    std::cout << "You ended the game. See you next time..." << std::endl;
}

int Game::errorHandler(int e)
{
    std::cout << "!ERROR: " << e << std::endl;
    quit = true;
    return e;
}

int Game::calculateActualPrice(int price)
{
    return std::rand() % (price + 1);
}

void Game::kill(int peopleToKill)
{
    if (stats.people > peopleToKill)
    {
        stats.peopleKilled += peopleToKill;
        stats.people -= peopleToKill;
    }
    else
    {
        stats.peopleKilled += stats.people;
        stats.people = 0;
        die();
    }
}

/// @brief Shows death message and sets game end flag.
void Game::die()
{
    quit = true;
    std::cout << "No one is left in your colony. You can't sustain yourself and therefore die." << std::endl;
}

Game::~Game()
{
    delete map;

    turn = 0;
};