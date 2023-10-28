cmake_minimum_required(VERSION 3.26)
project(DoomDepthsC_G21 C)

set(CMAKE_C_STANDARD 11)

add_executable(DoomDepthsC_G21 main.c
        Player.c
        Player.h
        Monster.c
        Monster.h
        fight.c
        heal.c
        level.c
        level.h
        level.h
)
