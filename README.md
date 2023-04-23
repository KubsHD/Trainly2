# Trainly

A C++ port/demake of a canceled game I worked on.

## Prerequisites
- Visual Studio 2022 with C++ and CMake support

## How to build
1. Clone the repository

	`git clone`

#### Automatic build
2. Open cloned directory with Visual Studio
<br />

#### Manual build

2. Create build directory<br />
`mkdir build && cd build`<br />
3. Generate project files manually<br />
`cmake .. -G "Ninja"`
4. Build<br />
`ninja`

## Used libraries
- [DirectXTK](https://github.com/microsoft/DirectXTK)
- [DirectXTex](https://github.com/microsoft/DirectXTex)
- [imgui](https://github.com/ocornut/imgui)
- [SDL2](https://github.com/libsdl-org/SDL)
- [assimp](https://github.com/assimp/assimp)


