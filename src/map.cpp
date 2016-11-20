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
        _load_meta("map.meta");
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

    std::string Map::get_param(std::string key)
    {
        return _meta[key];
    }

    void Map::_load_meta(std::string filename)
    {
        std::ifstream map(filename);

        std::string line;

        if (map.is_open()) {

            while (std::getline(map, line)) {
                // Comments
                if (line[0] == '#')
                    continue;

                auto pos = line.find("=");

                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1, line.length());

                _meta[key] = value;
            }

            map.close();
        } else {
            throw std::runtime_error("Map file not found");
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

            map.close();
        } else {
            throw std::runtime_error("Map file not found");
        }
    }
}
