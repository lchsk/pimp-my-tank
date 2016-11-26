#include <iostream>

#include "animation.h"

namespace pmt
{
    Animation::Animation(
        std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
        textures,
        std::vector<std::string>& texture_names)
    {
        reset();
        _init();

        for (auto& name : texture_names) {
            _frames.push_back(std::make_unique<sf::Sprite>(*textures[name].get()));
        }

        _frame_w = _frames[0]->getTexture()->getSize().x;
        _frame_h = _frames[0]->getTexture()->getSize().y;
    }

    Animation::Animation(
        std::unique_ptr<sf::Texture>& spritesheet,
        unsigned frames_count,
        unsigned frame_width,
        unsigned frame_height)
    {
        reset();
        _init();

        _frame_w = frame_width;
        _frame_h = frame_height;

        for (unsigned i = 0; i < frames_count; i++) {
            _frames.push_back(std::make_unique<sf::Sprite>(
                                  *spritesheet.get(),
                                  sf::IntRect(
                                      i * frame_width, 0, frame_width, frame_height
                                      )));
        }
    }

    Animation::~Animation()
    {

    }

    void Animation::update(sf::Time delta)
    {
        if (! _is_playing) return;

        if ((_since_update - delta).asSeconds() > (delta.asSeconds() / _speed)) {
            if (_frame >= _frames.size() - 1) {
                _frame = 0;

                _is_playing = _loop;
            } else
                _frame++;

            _since_update = sf::seconds(0);
        } else {
            _since_update += delta;
        }
    }

    void Animation::render(sf::RenderWindow& window)
    {
        if (_is_playing)
            window.draw(*_frames[_frame]);
    }

    sf::Vector2f Animation::get_size() const
    {
        sf::Vector2f vec;

        vec.x = _frame_w * _frames[0]->getScale().x;
        vec.y = _frame_h * _frames[0]->getScale().y;

        return vec;
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

    void Animation::loop(bool loop)
    {
        _loop = loop;
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

    void Animation::_init()
    {
        _speed = 1.0f;
        _frame = 0;
        _loop = false;
    }
}
