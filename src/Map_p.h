#include <vector>
#include <iostream>
#include <random>
#include <string>

enum DIRECTION
{
    DIRECTION_NORTH,
    DIRECTION_EAST
};

enum TILE
{
    TILE_EMPTY,
    TILE_WATER,
    TILE_STONE,
    TILE_NUMBER
};

class Map
{
private:
    std::vector<int> *map;

public:
    Map();

    int exploreTile(int dir);

    int getMaxPrice(int dir);

    ~Map();
};
