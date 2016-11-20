#include <iostream>
#include <fstream>

#include "map.h"
#include "util.h"

namespace pmt
{
    Map::Map(
        std::unordered_map<int, std::string> tiles,
        std::unordered_map
            <std::string, std::unique_ptr<sf::Texture> >& textures)
    {
        _load_data(DIR_MAPS + "map.data", tiles, textures);
        _load_meta(DIR_MAPS + "map.meta");

        for (int i = 0; i < 8; i += 2) {
            std::string filename = "trees" + std::to_string(i / 2 + 1) + ".png";

            _bg[i] = std::make_unique<sf::Sprite>(
                *textures[filename].get());
            _bg[i + 1] = std::make_unique<sf::Sprite>(
                *textures[filename].get());

            _bg[i + 1]->setPosition(BG_WIDTH, 0);
        }

        _bg[2]->setPosition(0, 50);
        _bg[3]->setPosition(BG_WIDTH, 50);
        _bg[4]->setPosition(0, 70);
        _bg[5]->setPosition(BG_WIDTH, 70);
        _bg[6]->setPosition(0, 90);
        _bg[7]->setPosition(BG_WIDTH, 90);

        _bg_speeds[0] = .02;
        _bg_speeds[1] = .023;
        _bg_speeds[2] = .027;
        _bg_speeds[3] = .03;
    }

    Map::~Map()
    {

    }

    void Map::update(sf::Time delta)
    {
        for (int i = 0; i < 8; i += 2) {
            double move_x = _bg_speeds[i / 2] * delta.asMilliseconds();

            _bg[i]->setPosition(_bg[i]->getPosition().x - move_x, _bg[i]->getPosition().y);
            _bg[i + 1]->setPosition(_bg[i + 1]->getPosition().x - move_x, _bg[i + 1]->getPosition().y);

            int larger, smaller;

            if (_bg[i]->getPosition().x > _bg[i + 1]->getPosition().x) {
                larger = i;
                smaller = i + 1;
            } else {
                larger = i + 1;
                smaller = i;
            }

            _bg[larger]->setPosition(_bg[smaller]->getPosition().x + BG_WIDTH, _bg[smaller]->getPosition().y);

            if (_bg[smaller]->getPosition().x <= -BG_WIDTH)
                _bg[smaller]->setPosition(BG_WIDTH, _bg[smaller]->getPosition().y);
        }
    }

    void Map::render(sf::RenderWindow& window)
    {
        for (int i = 0; i < 8; i++) {
            window.draw(*_bg[i]);
        }

        for (sf::Sprite& s : _tiles) {
            window.draw(s);
        }
    }

    std::string Map::get_param(std::string key)
    {
        return _meta[key];
    }

    bool Map::check_collision(std::shared_ptr<pmt::Bullet>& bullet)
    {
        if (bullet->is_flying()) {
            for (auto& tile : _tiles) {
                if (tile.getGlobalBounds().intersects(
                        bullet->get_sprite()->getGlobalBounds())) {
                    bullet->hit();
                    return true;
                }
            }

            sf::Vector2f pos = bullet->get_sprite()->getPosition();

            if (pos.x > pmt::config::WINDOW_W
                || pos.x < 0
                || pos.y < 0
                || pos.y > pmt::config::WINDOW_H) {
                bullet->hit();
                return true;
            }
        }

        return false;
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
