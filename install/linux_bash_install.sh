#!/bin/bash

sudo apt install libharfbuzz-dev libfreetype-dev

mkdir -p $HOME/SDL
git clone https://github.com/libsdl-org/SDL.git $HOME/SDL
cd $HOME/SDL

cmake -Bbuild -DSDL_STATIC=OFF -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build
cmake --install build

mkdir -p $HOME/SDL_ttf
git clone https://github.com/libsdl-org/SDL_ttf.git $HOME/SDL_ttf
cd $HOME/SDL_ttf

cmake -Bbuild -DSDL_STATIC=OFF -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build
cmake --install build

mkdir -p $HOME/dungeon-game-client
git clone https://github.com/ZenithMeetsNadir/dungeon-game-client.git $HOME/dungeon-game-client
cd $HOME/dungeon-game-client

cmake -Bbuild -DCMAKE_INSTALL_PREFIX=$HOME
cmake --build build
cmake --install build