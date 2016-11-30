# pimp my tank

A video game created for GitHub Game Off 2016 (Theme is Hacking, Modding, or Augmenting).

The aim of the game is to control your tank and hit other players' (human or computer controlled) tanks. The user the *modify* his/her tank (by buying new weapons, changing tank's colour) as well as alter the environment (by changing the wind speed which makes shooting missiles quite tricky).

# Features
* several maps included (which allow to play in various configurations, e.g. human vs computer vs computer, or even watch computer players fighting each other)
* maps are stored in text files, so it's easy to create more maps without any programming
* pixel-style graphics
* allows to play against a computer or against human players using the same machine
* computer players have different skills levels
* a player can buy various items in a shop (e.g. three types of missiles, upgrades)
* written in C++14 and intended for Linux

# Dependencies
* C++14 compiler (e.g. g++6), tested with g++ 6.1.1
* SFML library, v2.3.2
* Linux, tested on Ubuntu 14.04

# Installation
After cloning the repository run:

    $ ./compile_and_run.sh

which will compile the game and start it automatically.

Start the game from the terminal:

    $ ./pimp-my-tank

(Remember to `cd` into the directory where the `pimp-my-tank` binary is located`

# Controls
|Key   |Action   |
|---|---|
|<space>   |press to shoot   |
|<left> / <right>                |change weapon   |
|<up> / <down>                   |alter angle of the gun   |
|S                               |open / close shop|
|<return>                        |buy in a shop / confirm|
|N                               |move to the next turn  |
|<control>                       |spin around the tank   |
|<esc>                           |open menu / go back    |
