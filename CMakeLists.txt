cmake_minimum_required(VERSION 2.8)

project(StorbeedEngine)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SUPPRESS_REGENERATION true)
#set(CMAKE_UNITY_BUILD ON)

if (WIN32)
    add_compile_definitions(WIN)
endif (WIN32)

#assimp
include_directories(deps/assimp/include)
add_subdirectory(deps/assimp)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#sdl
include_directories(deps/SDL2/include)
add_definitions(-DSDL_STATIC)
add_subdirectory(deps/SDL2)
remove_definitions(-DUSING_GENERATED_CONFIG_H)

#directxtex 
include_directories(deps/DirectXTex/DirectXTex)
add_subdirectory(deps/DirectXTex)

#directxtk
include_directories(deps/DirectXTK/Inc)
add_subdirectory(deps/DirectXTK)




set_target_properties(assimp PROPERTIES FOLDER "deps")
set_target_properties(DirectXTK PROPERTIES FOLDER "deps")
set_target_properties(DirectXTex PROPERTIES FOLDER "deps")

# tools

add_subdirectory(tools/DataPacker)
set_target_properties(DataPacker PROPERTIES FOLDER "tools")

# game +  engine

include_directories(Trainly)
add_subdirectory(Trainly)




