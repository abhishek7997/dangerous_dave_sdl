# Dangerous Dave C++ SDL
Dangerous Dave implementation using C++ 11 and SDL 2.0

## Overview

This project is a single-player 2D platformer implemented in C++ using the SDL library. It is a re-implementation of the original Dangerous Dave DOS game.
- Compiler used: MinGW g++ 6.3.0
- C++ version: C++ 11

<br>
<img src="https://github.com/abhishek7997/dangerous_dave_sdl/assets/68701271/4609af71-f687-4234-8a81-788a934c98e6" width=512>
<br>
<img src="https://github.com/abhishek7997/dangerous_dave_sdl/assets/68701271/8cc256ac-0e60-4818-aec8-7471bf2c5885" width=512>

## Info

- Contains most features of the original
- First 3 levels implemented
- Level format as a text file

## Getting started

### Prerequisites

- C++ compiler (g++)
- SDL library

### Building
1. Clone the repository
2. Modify the paths:
   1. In `TileManager.hpp`, change `m_TilemapPath` to the path where tilesheet bmp in assets folder is present.
   2. In `build.bat`, modify the paths of SDL, SDL_ttf and SDL_Image.
3. Run `build.bat`. The executable will be created in the `bin` folder.
4. The bin folder should contain `SDL2.dll`, `SDL2_ttf.dll` and `SDL2_image.dll` to run the compiled executable.
