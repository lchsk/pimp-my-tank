#ifndef ANIMATION_H
#define ANIMATION_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace pmt
{
    class Animation
    {
    public:
        Animation(
            std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
            textures,
            std::vector<std::string>& texture_names);
        ~Animation();

        void render(sf::RenderWindow& window);
        void update(sf::Time delta);

        bool is_playing();
        void play();
        void stop();
        void reset();
        void set_speed(double speed);
        void set_position(double x, double y);
        void set_scale(double x, double y);

    private:
        std::vector<std::unique_ptr<sf::Sprite> > _frames;
        sf::Time _since_update;

        double _speed;
        unsigned _frame;
        bool _is_playing;
        sf::Vector2f _pos;
        sf::Vector2f _scale;
    };
}

#endif
