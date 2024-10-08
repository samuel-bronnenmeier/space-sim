#include <Game_p.h>

Game::Game()
{
    turn = 1;

    stats.people = STARTING_PEOPLE;
    stats.food = STARTING_FOOD;
    stats.water = STARTING_WATER;
    stats.stone = 0;

    stats.d_food = 0;
    stats.d_water = 0;
    stats.d_stone = 0;

    stats.peopleKilled = 0;

    map = new Map;

    playerName = "";

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
    std::cout << "----------------------------------" << std::endl
              << "Space Colony Simulator" << std::endl
              << "----------------------------------" << std::endl
              << "Humanity has made it. They made Earth a planet almost uninhabitable and certainly no place on would want to be." << std::endl
              << "Fortunately, technology has come far in the meantime, enabling space exploration missions, which have lately even been successful." << std::endl
              << "A few planets not too dissimilar from Earth have been found, and the Internation Space Agency (ISA) decided to send humans up there in order to colonize them." << std::endl
              << "But as there are not enough specalists on Earth, these 'colony starters' are organized by several AIs, one of which is you." << std::endl
              << "(Press enter to continue)" << std::endl;
    std::cin.ignore();
    std::cout << "You just landed on your new home planet and your colony is motivated to make a home out of it." << std::endl
              << "You are starting with " << STARTING_PEOPLE << " colonists." << std::endl
              << "In the storage room " << STARTING_FOOD << " units of food and " << STARTING_WATER << " units of water are left." << std::endl
              << "It looks like the planet's surface could be harvested for stone. The radar also states more water and food could be found there." << std::endl
              << std::endl
              << "Type in the number of your wished action and press enter." << std::endl
              << "If this is your first time here you might want to start with the help feature, which explains every action you can take." << std::endl
              << std::endl;
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
    int inp;
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
        mainHelp();
        inp = getInput();
        handleInput(inp);
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
                std::cout << "The remaining part of the group managed to brings back " << price << " stones." << std::endl;
                stats.stone += price;
                break;

            default:
                errorHandler(ERROR_INPUT_HANDLING);
                break;
            }
        }
        else
        {
            stats.peopleKilled += stats.people - killed;
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

        stats.stone -= stonePrice;

        if (!dead)
        {
            switch (building)
            {
            case 0:
                stats.d_water += stonePrice / 1.5;
                std::cout << "You built a well. " << price << " people lost their life..." << std::endl;
                break;

            case 1:
                stats.d_food += stonePrice / 1.5;
                std::cout << "You built a plant house. " << price << " died during construction..." << std::endl;
                break;

            case 2:
                stats.d_stone += stonePrice / 1.5;
                std::cout << "You built a quarry. " << price << " people have gone lost somewhere..." << std::endl;
                break;

            default:
                errorHandler(ERROR_INPUT_HANDLING);
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
    std::cout << "You want to extract something from the ground. This is and really dangerous and could go wrong..." << std::endl
              << "What do you want to extract? (0=food, 1=water, 2=stone)" << std::endl;

    int ressource;
    std::cin >> ressource;

    int price;
    while (true)
    {
        std::cout << "How many people do you want to send? Your colony currently holds " << stats.people << " people..." << std::endl;
        std::cin >> price;

        if (price > stats.people)
        {
            std::cout << "Your colony isn't that big..." << std::endl;
        }
        else
        {
            break;
        }
    }

    std::cout << "Every person that doesn't die could carry a unit if it finds one." << std::endl
              << "Do you want to do it? (0=no, 1=yes)" << std::endl;

    int confirm = 0;
    std::cin >> confirm;
    if (confirm)
    {
        int actualPrice = calculateActualPrice(price);
        kill(actualPrice);

        int amount = price - actualPrice;

        if (!dead && (amount != 0))
        {
            int actualAmount = rand() % (amount + 1);

            switch (ressource)
            {
            case 0:
                stats.food += actualAmount;
                std::cout << "Some colonists came back with " << actualAmount << " food. " << actualPrice << " people lost their life..." << std::endl;
                break;

            case 1:
                stats.water += actualAmount;
                std::cout << actualAmount << " units of water have been found. " << actualPrice << " didn't make it..." << std::endl;
                break;

            case 2:
                stats.stone += actualAmount;
                std::cout << "Some stone has been dug up. " << actualPrice << " people have gone lost somewhere..." << std::endl;
                break;

            default:
                errorHandler(ERROR_INPUT_HANDLING);
                break;
            }
        }
        else
        {
            std::cout << "Seems like no one came back..." << std::endl;
        }
    }
    else
    {
        std::cout << "You decide to not build anything..." << std::endl;
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
        // Colonists' reproduction
        int d_people = stats.people / 6;
        if (d_people < 1)
        {
            d_people = 1;
        }
        stats.people += d_people;
        std::cout << "Your colony grew a bit as " << d_people << " people were made..." << std::endl;

        // Buildings producing
        stats.food += stats.d_food;
        stats.water += stats.d_water;
        stats.stone += stats.d_stone;

        // Eating and drinking
        std::cout
            << "Your colony is drinking and eating..." << std::endl;

        int foodNeeded = stats.people / 2;
        int waterNeeded = stats.people;

        if (stats.food < foodNeeded)
        {
            std::cout << "Some people didn't get something to eat. They died." << std::endl;
            kill(foodNeeded - stats.food);
            stats.food = 0;
        }
        else
        {
            stats.food -= foodNeeded;
        }

        if (stats.water < waterNeeded)
        {
            std::cout << "Some people didn't get something to drink. They died." << std::endl;
            kill(waterNeeded - stats.water);
            stats.water = 0;
        }
        else
        {
            stats.water -= waterNeeded;
        }
    }
}

void Game::displayStats(bool endOfTurn)
{

    std::cout << std::endl
              << "-----------------------------" << std::endl;

    if (endOfTurn)
    {
        std::cout << "At the end of turn " << turn << " your stats are as following:" << std::endl;
    }
    else
    {
        std::cout << "You are currently in turn " << turn << "." << std::endl;
    }

    std::cout << "People: " << stats.people << std::endl;
    std::cout << "Food: " << stats.food << " ... +" << stats.d_food << "/turn" << std::endl;
    std::cout << "Water: " << stats.water << " ... +" << stats.d_water << "/turn" << std::endl;
    std::cout << "Stone: " << stats.stone << " ... +" << stats.d_stone << "/turn" << std::endl;

    std::cout << "You killed " << stats.peopleKilled << " people so far." << std::endl;

    std::cout << "-----------------------------" << std::endl
              << std::endl;

    if (!endOfTurn)
    {
        int input = getInput();

        handleInput(input);
    }
}

void Game::displayOutro()
{
    if (dead)
    {
        std::cout << "No colonist is left. You can't sustain yourself and die. This planet will not be a future home for humanity..." << std::endl;
        std::cout << "Your colony survived " << turn << " turns, during which a total of " << stats.peopleKilled << " people died on your behalf..." << std::endl;
        if (stats.peopleKilled > 50)
        {
            std::cout << "Maybe killing all those colonists was your goal all along?" << std::endl
                      << "We will probably never know..." << std::endl;
        }
        std::cout << "The game ends here, reality begins. See you next time..." << std::endl;
    }
    else
    {
        std::cout << "You abandon your colony and end the game for now. We hope to hear from you again...";
    }
}

int Game::errorHandler(int e)
{
    std::string msg;

    switch (e)
    {
    case ERROR_INPUT_HANDLING:
        msg = "Faulty input. Please be more careful next time.";
        break;

    default:
        quit = true;
        msg = "Unknown error...";
        break;
    }

    std::cout << "!ERROR: " << msg << std::endl;

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