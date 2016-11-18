#ifndef BULLET_MGR_H
#define BULLET_MGR_H

#include <cmath>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace pmt
{
    enum class BulletType {
        Missile
    };

    class Bullet
    {
    public:
        Bullet();
        ~Bullet();

    private:
        std::unique_ptr<sf::Sprite> _sprite;
        bool _active;
        BulletType _type;
    };

    class BulletMgr
    {
        public:
            BulletMgr(const BulletMgr&) = delete;
            BulletMgr& operator=(const BulletMgr&) = delete;

            BulletMgr();
            ~BulletMgr();

            void run();
            void update(sf::Time delta);
            void render(sf::RenderWindow& window);

        private:
            void _simulate();

            std::vector<pmt::Bullet> _bullets;
    };
}

#endif
