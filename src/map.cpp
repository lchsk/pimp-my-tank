#include <iostream>
#include <fstream>

#include "map.h"

namespace pmt
{
    Map::Map()
    {
        _load_data("map.data");
    }

    Map::~Map()
    {

    }

    void Map::render(sf::RenderWindow& window)
    {
    }

    void Map::_load_data(std::string filename)
    {
        _tiles.clear();

        std::ifstream map(filename);

        if (map.is_open()) {
            int tile;

            while (! map.eof()) {
                map >> tile;
                _tiles.push_back(tile);
            }
        } else {
            throw std::runtime_error("Map file not found");
        }
    }
}
