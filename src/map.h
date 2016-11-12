#ifndef MAP_H
#define MAP_H

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace pmt
{
    class Map
    {
    public:
        Map();
        ~Map();

        void render(sf::RenderWindow& window);

    private:
        void _load_data(std::string filename);
        std::vector<int> _tiles;
    };
}

#endif
