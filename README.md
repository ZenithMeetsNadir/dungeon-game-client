# Dungeon game lacking a name (yet)

Info will be dumped later... now to the installation guide!

## Installation guide

Hate to say that, but there is no way other than building the source... yeeey!

### Windows

Required tools: `gcc`, `cmake`, `git`, `vcpkg`

### Linux

Required tools: `gcc`, `cmake`, `git`

#### Step by step installation

1. install SDLs

    The following will install SDLs into `$HOME/.local`

    Make sure to install font rendering libraries for `SDL_ttf` first:

        sudo apt install libharfbuzz-dev libfreetype-dev
    
    Clone SDLs:

        git clone https://github.com/libsdl-org/SDL.git
        git clone https://github.com/libsdl-org/SDL_ttf.git
        
    Then in both SDL repositories build and install using cmake:

        cmake -Bbuild -DSDL_STATIC=OFF -DCMAKE_INSTALL_PREFIX=$HOME/.local
        cmake --build build
        cmake --install build

2. Install the game!

    Clone game repository:

        git clone https://github.com/ZenithMeetsNadir/dungeon-game-client.git

    The following will install the game directly to path `${your-desired-path}` you provide:

        cmake -Bbuild -DCMAKE_INSTALL_PREFIX=${your-desired-path}
        cmake --build build
        cmake --install build