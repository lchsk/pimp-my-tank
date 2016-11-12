#ifndef MAP_H
#define MAP_H

#include <memory>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace pmt
{
    class Map
    {
    public:
        Map(std::unordered_map<int, std::string> tiles,
            std::unordered_map
                <std::string, std::unique_ptr<sf::Texture> >& textures
        );
        ~Map();

        void render(sf::RenderWindow& window);

    private:
        void _load_data(
            std::string filename,
            std::unordered_map<int, std::string> tiles,
            std::unordered_map
                <std::string, std::unique_ptr<sf::Texture> >& textures
        );

        std::vector<sf::Sprite> _tiles;
    };
}

#endif
