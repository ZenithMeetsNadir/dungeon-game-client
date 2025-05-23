# Dungeon game lacking a name (yet)

Info will be dumped later... now to the installation guide!

## Installation guide

Hate to say that, but there is no way other than building the source... yeeey!

Required tools: `git`, `cmake`

### Step by step installation

1. Clone game repository:

        git clone https://github.com/ZenithMeetsNadir/dungeon-game-client.git

    Check its directory and proceed with platform dependent installation.


#### Windows

Additional required tools: `mingw`

2. Create build files:

        cmake -B build -D CMAKE_INSTALL_PREFIX=. -G MinGW Makefiles"

#### Linux

2. Create build files:

        cmake -B build -D CMAKE_INSTALL_PREFIX=.


### Continue platform independent step by step installation

3. Build and install:

        cmake --build build
        cmake --install build

    This installs the binary executable in the top level of the repository. If you wish to provide a custom installation path, modify the `CMAKE_INSTALL_PREFIX` in the second step to your likings.