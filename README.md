# Dangerous Dave C++ SDL
Dangerous Dave implementation using C++ 11 and SDL 2.0

## Overview

This project is a single-player 2D platformer implemented in C++ using the SDL library. It is a re-implementation of the original Dangerous Dave DOS game.
- Compiler used: MinGW g++ 6.3.0
- C++ version: C++ 11

<br>
<img src="https://github.com/abhishek7997/dangerous_dave_sdl/assets/68701271/caf5b9f7-e420-4239-b3ad-391561f02d80" width=512>
<br>
<img src="https://github.com/abhishek7997/dangerous_dave_sdl/assets/68701271/ec61b9d3-f70c-4512-a8f1-9dca5b01516e" width=512>

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
