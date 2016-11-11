#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "tank.h"

namespace ks
{
    class Game
    {
        public:
            Game(const Game&) = delete;
            Game& operator=(const Game&) = delete;

            Game();
            ~Game();

            void run();

        private:
            void handle_events();
            void update(sf::Time delta);
            void render();

            std::shared_ptr<ks::Tank> _player;

            std::unique_ptr<sf::RenderWindow> _window;

            std::vector<std::string> _filenames;
            std::unordered_map<std::string, std::unique_ptr<sf::Texture> >
                _textures;
    };
}
#endif
