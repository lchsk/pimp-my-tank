#include <iostream>
#include <fstream>

#include "map.h"

namespace pmt
{
    Map::Map(
        std::unordered_map<int, std::string> tiles,
        std::unordered_map
            <std::string, std::unique_ptr<sf::Texture> >& textures)
    {
        _load_data("map.data", tiles, textures);
    }

    Map::~Map()
    {

    }

    void Map::render(sf::RenderWindow& window)
    {

    }

    void Map::_load_data(
        std::string filename,
        std::unordered_map<int, std::string> tiles,
        std::unordered_map
            <std::string, std::unique_ptr<sf::Texture> >& textures)
    {
        _tiles.clear();

        std::ifstream map(filename);

        if (map.is_open()) {
            int tile;

            while (! map.eof()) {
                map >> tile;

                // Omit tile ID=0
                if (! tile)
                    continue;

                auto& tex = textures[tiles[tile]];

                _tiles.push_back(sf::Sprite(*tex.get()));
            }
        } else {
            throw std::runtime_error("Map file not found");
        }
    }
}
