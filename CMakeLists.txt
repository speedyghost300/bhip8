cmake_minimum_required(VERSION 3.24)
project(bhip8 C)

set(CMAKE_C_STANDARD 11)

set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE          
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL3)

add_executable(BHIP8
    main.c
    cpu.c
    display.c
    ram.c
 keypad.c
)

target_link_libraries(BHIP8 PRIVATE SDL3::SDL3)