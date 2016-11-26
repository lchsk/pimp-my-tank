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

        Animation(
            std::unique_ptr<sf::Texture>& spritesheet,
            unsigned frames_count,
            unsigned frame_width,
            unsigned frame_height);

        ~Animation();

        void render(sf::RenderWindow& window);
        void update(sf::Time delta);

        bool is_playing();
        void play();
        void stop();
        void reset();
        void loop(bool loop);
        void set_speed(double speed);
        void set_position(double x, double y);
        void set_scale(double x, double y);
        sf::Vector2f get_size() const;

    private:
        std::vector<std::unique_ptr<sf::Sprite> > _frames;
        sf::Time _since_update;

        void _init();

        double _speed;
        unsigned _frame;
        unsigned _frame_w;
        unsigned _frame_h;

        bool _is_playing;
        bool _loop;
        sf::Vector2f _pos;
        sf::Vector2f _scale;
    };
}

#endif
