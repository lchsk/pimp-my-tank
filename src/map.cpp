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
        for (sf::Sprite& s : _tiles) {
            window.draw(s);
        }
    }

    void Map::_load_data(
        std::string filename,
        std::unordered_map<int, std::string> tiles,
        std::unordered_map
            <std::string, std::unique_ptr<sf::Texture> >& textures)
    {
        _tiles.clear();

        std::ifstream map(filename);

        int tile_no = -1;

        if (map.is_open()) {
            int tile;

            while (! map.eof()) {
                tile_no++;

                map >> tile;

                // Omit tile ID=0
                if (! tile)
                    continue;

                auto& tex = textures[tiles[tile]];

                sf::Sprite sprite(*tex.get());

                int row = tile_no / 25;
                int col = tile_no % 25;

                sprite.setPosition(col * 32, row * 32);

                _tiles.push_back(sprite);
            }
        } else {
            throw std::runtime_error("Map file not found");
        }
    }
}
