#include <Help_p.h>

void mainHelp()
{
    std::cout << std::endl
              << "-/-/-/-/-/-/-/-/-/-" << std::endl
              << "What topic do you want help on? Press the corresponding number and then enter." << std::endl
              << "(0=general help, 1=exploring, 2=building, 3=extracting)" << std::endl;
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

    default:
        std::cout << "You either did something wrong or know what you're doing." << std::endl
                  << "If former is the case, just ask for help again by pressing " << CONTROL_HELP << " and then enter." << std::endl;
        break;
    }
}

void generalHelp()
{
}

void exploringHelp()
{
}

void buildingHelp()
{
    std::cout << "There are three kinds of buildings. They all work the same but produce different kinds of goods:" << std::endl
              << "1. The well produces water." << std::endl
              << "2. The plant house produces food." << std::endl
              << "3. The quarry produces stone." << std::endl
              << "During the process of building them people can die. The bigger you build, the more colonists are in danger." << std::endl;
}

void extractingHelp()
{
}