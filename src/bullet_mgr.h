#ifndef BULLET_MGR_H
#define BULLET_MGR_H

#include <cmath>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace pmt
{
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
    };
}

#endif
