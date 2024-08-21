#include <Map_p.h>

Map::Map()
{
    map = new std::vector<int>[4];
}

int Map::exploreTile(int dir)
{
    int tile = std::rand() % (TILE_NUMBER);

    map[dir].push_back(tile);

    return tile;
}

int Map::getMaxPrice(int dir)
{
    return (map[dir].size() + 1) * 2;
}

Map::~Map()
{
}