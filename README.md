# Trainly

A C++ port/demake of a game I worked on.

## How to play

- `R` to rotate rail
- `Space` to place rail
- `Backtick` to enter debug mode

## Prerequisites
- Visual Studio 2022 with C++ and CMake support

## How to build
1. Clone the repository `git clone --recurse-submodules https://github.com/KubsHD/Trainly2`

#### Automatic build
2. Open cloned directory with Visual Studio
#### Manual build

2. Create build directory: `mkdir build && cd build`
3. Generate project files manually: `cmake .. -G "Ninja"`
4. Build: `ninja`

## Used libraries
- [DirectXTK](https://github.com/microsoft/DirectXTK)
- [DirectXTex](https://github.com/microsoft/DirectXTex)
- [imgui](https://github.com/ocornut/imgui)
- [SDL2](https://github.com/libsdl-org/SDL)
- [assimp](https://github.com/assimp/assimp)


