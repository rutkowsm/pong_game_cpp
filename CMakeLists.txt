cmake_minimum_required(VERSION 3.25)
project(testProject)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR C:/SDL/LibSDL/VC/SDL2/include)
set(SDL2_LIB_DIR C:/SDL/LibSDL/VC/SDL2/lib/x64)

set(SDL2MIXER_INCLUDE_DIR C:/SDL/LibSDL/VC/SDL2_mixer/include)
set(SDL2MIXER_LIB_DIR C:/SDL/LibSDL/VC/SDL2_mixer/lib/x64)

set(SDL2TTF_INCLUDE_DIR C:/SDL/LibSDL/VC/SDL2_ttf/include)
set(SDL2TTF_LIB_DIR C:/SDL/LibSDL/VC/SDL2_ttf/lib/x64)



include_directories(${SDL2_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} ${SDL2_LIB_DIR}/SDL2.lib ${SDL2_LIB_DIR}/SDL2main.lib ${SDL2MIXER_LIB_DIR}/SDL2_mixer.lib ${SDL2TTF_LIB_DIR}/SDL2_ttf.lib)


