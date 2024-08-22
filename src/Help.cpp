#include <Help_p.h>

void mainHelp()
{
    std::cout << std::endl
              << "-/-/-/-/-/-/-/-/-/-" << std::endl
              << "What topic do you want help on? Press the corresponding number and then enter." << std::endl
              << "(0=general help, 1=exploring, 2=building, 3=extracting, 4=ressources)" << std::endl;
    int inp = 0;
    std::cin >> inp;
    switch (inp)
    {
    case 0:
        generalHelp();
        break;

    case 1:
        exploringHelp();
        break;

    case 2:
        buildingHelp();
        break;

    case 3:
        extractingHelp();
        break;

    case 4:
        ressourceHelp();
        break;

    default:
        std::cout << "You either did something wrong or know what you're doing." << std::endl
                  << "If former is the case, just ask for help again by pressing " << CONTROL_HELP << " and then enter." << std::endl;
        break;
    }
}

void generalHelp()
{
    std::cout << "The game is based on turns. Each turn you can do one out of three things:" << std::endl
              << "1. Explore the planet and possibly find ressources." << std::endl
              << "2. Build something that produces ressources every turn." << std::endl
              << "3. Extract ressources from the planet'surface (or underground)." << std::endl
              << "Every one of these actions could lose you some colonists. You can find more information on these topics in the other help tabs." << std::endl
              << "Every turn the colonists reproduce through some sort of labratory (you're not sure how, but they produce fully grown-ups)." << std::endl
              << "Other 'actions' you can do are looking at your stats (i.e. amount of ressources your colony owns), quitting the game or asking for help again." << std::endl
              << "(Press enter to continue)" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}

void exploringHelp()
{
    std::cout << "If you're feeling brave and lucky, exploring is your way to easily come to everything except food." << std::endl
              << "Be careful though, as each exploration could quite possibly lose you some colonists. The further out you try to go, the more lives are on stake." << std::endl
              << "But with each step you take in each direction the yields will also grow. Have fun discovering the planet..." << std::endl
              << "(Press enter to continue)" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}

void buildingHelp()
{
    std::cout << "There are three kinds of buildings. They all work the same but produce different kinds of goods:" << std::endl
              << "1. The well produces water." << std::endl
              << "2. The plant house produces food." << std::endl
              << "3. The quarry produces stone." << std::endl
              << "During the process of building them people can die. The bigger you build, the more colonists are in danger." << std::endl
              << "(Press enter to continue)" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}

void extractingHelp()
{
    std::cout << "Extraction is the easiest way to get ressources and you can get any one of them this way." << std::endl
              << "It is very dangerous though, so use it carefully in order to keep your colonists alive... " << std::endl
              << "(Press enter to continue)" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}

void ressourceHelp()
{
    std::cout << "There are three kinds of ressources:" << std::endl
              << "1. Water is the base of all life, including human. Each colonist uses up one unit of water each turn. Get water by exploring, extracting or building a well." << std::endl
              << "2. Food is equally important to keep your colonists alive. One unit of food feeds two colonists for a turn. Get it through extraction or a plant house." << std::endl
              << "3. Stone is not essential for living but will make managing your colony much easier as you can construct buildings only with stone. Get some by exploring, extracting or building a quarry." << std::endl
              << "Each of them is important in their own way and you will want to carefully create an equilibrium between them..." << std::endl
              << "(Press enter to continue)" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}