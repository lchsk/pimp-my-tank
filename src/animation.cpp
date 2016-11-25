#include <iostream>

#include "animation.h"

namespace pmt
{
    Animation::Animation(
        std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
        textures,
        std::vector<std::string>& texture_names
    ) :
        _speed(1.0f),
        _frame(0)
    {
        reset();

        for (auto& name : texture_names) {
            _frames.push_back(std::make_unique<sf::Sprite>(*textures[name].get()));
        }
    }

    Animation::~Animation()
    {

    }

    void Animation::update(sf::Time delta)
    {
        if (! _is_playing) return;

        if ((_since_update - delta).asSeconds() > (delta.asSeconds() / _speed)) {
            if (_frame >= _frames.size() - 1)
                _frame = 0;
            else
                _frame++;

            _since_update = sf::seconds(0);
        } else {
            _since_update += delta;
        }
    }

    void Animation::render(sf::RenderWindow& window)
    {
        window.draw(*_frames[_frame]);
    }

    void Animation::set_position(double x, double y)
    {
        _pos = sf::Vector2f(x, y);

        for (auto& frame : _frames) {
            frame->setPosition(_pos);
        }
    }

    void Animation::set_scale(double x, double y)
    {
        _scale = sf::Vector2f(x, y);

        for (auto& frame : _frames) {
            frame->setScale(_scale);
        }
    }

    bool Animation::is_playing()
    {
        return _is_playing;
    }

    void Animation::play()
    {
        _is_playing = true;
    }

    void Animation::stop()
    {
        _is_playing = false;
    }

    void Animation::reset()
    {
        _is_playing = false;
        _since_update = sf::seconds(99999);
    }

    void Animation::set_speed(double speed)
    {
        _speed = speed;
    }
}
