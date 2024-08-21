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
    dead = false;
}

int Game::run()
{
    srand(time(0));

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
        build();
        break;

    case CONTROL_EXTRACT:
        extract();
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
    std::cout << "You decided to go exploring unknown parts of the planet, possibly leaving some of your colonists out there..." << std::endl;
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

void Game::build()
{

    std::cout << "You decided to build something on the cost of stone and probably some lives..." << std::endl;
    std::cout << "What do you want to build? (0=well, 1=plant house, 2=quarry, 3=descriptions)" << std::endl;

    int building;
    std::cin >> building;

    if (building == 3)
    {
        buildingHelp();
        build();
        return;
    }

    int stonePrice = 0;

    while (true)
    {
        std::cout << "With how much stone do you want to build? You have " << stats.stone << " stone..." << std::endl;
        std::cin >> stonePrice;

        if (stonePrice > stats.stone)
        {
            std::cout << "You don't have that much stone." << std::endl;
        }
        else
        {
            break;
        }
    }

    int peoplePrice = stonePrice / 2;

    std::cout << "With that much stone being used, up to " << peoplePrice << " people could die. The building would produce " << (int)(stonePrice / 1.5) << " units per turn." << std::endl;
    std::cout << "Do you want to do build it? (no=0, yes=1)" << std::endl;

    int confirm = 0;
    std::cin >> confirm;
    if (confirm)
    {
        int price = calculateActualPrice(peoplePrice);
        kill(price);

        if (!dead)
        {
            switch (building)
            {
            case 0:
                stats.d_water += stonePrice / 1.5;
                std::cout << "You built a well. " << price << " people lost their life..." << std::endl
                          << std::endl;
                break;

            case 1:
                stats.d_food += stonePrice / 1.5;
                std::cout << "You built a plant house. " << price << " died during construction..." << std::endl
                          << std::endl;
                break;

            case 2:
                stats.d_stone += stonePrice / 1.5;
                std::cout << "You built a quarry. " << price << " people have gone lost somewhere..." << std::endl
                          << std::endl;
                break;

            default:
                break;
            }
        }
    }
    else
    {
        std::cout << "You decide to not build anything..." << std::endl;
        int input = getInput();
        handleInput(input);
    }
}

void Game::extract()
{
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
    if (dead)
    {
        std::cout << "No colonist is left. You can't sustain yourself and die." << std::endl;
    }
    std::cout << "The game ended. See you next time..." << std::endl;
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
    dead = true;
}

Game::~Game()
{
    delete map;

    turn = 0;
};