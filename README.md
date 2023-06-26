# Space Invaders

![alt text](https://github.com/mahmoudi-1798/Space-Invaders/blob/master/resources/Game.png?raw=true)

## Final Project Of ( Advanced Programming ) - Spring of 2023
From the beginning of my programming journey, I was so passionate about game development, so a few weeks back, I started to learn SDL2. 
<br>So I made a recreation of classic Space Invaders for the final project of Advanced programming.  

## Requirements 
This implementation has external dependencies on the following libraries:
- SDL2
- SDL2_ttf
- SDL2_image

## Installation guide
- For <b>Debian-based</b> systems run command below to install SDL2 and everything necessary to build programs:
```
sudo apt-get install libsdl2-dev
```
- For <b>Red Hat-based</b> systems runcommand below to install SDL2 and everything necessary to build programs:
```
sudo dnf install SDL2-devel
```

## Course discription

## Overview
- common.h: including base headers and declare extern variables 
- base.h: declaring base classes. 
    - App: main class of the game
    - Entity: use this class to create entities in the game like player.
    - Interval: 
- defs.h: defined global variables.
- init.h: initialize & prepare the SDL.
- draw.h:  declare some functions to simplify drawing part of the game.
- enemies.h: create enemies, bullets and charechteristics of enemies.
- handlingPlayer.h: create player bullets and handeling player movements. 
- input.h: handle inputs and assign functionallity to the keys.
- scenes.h: handling special scenes in game such as <b>GAME OVER</b>.
- underLine.h: drawing the green line in the game.
- walls.h: handling walls in game.

### main.cpp 
- `main()`: main function contains the main game loop.
- `renderMenu()`: we use it to render menu page and show it by `displayMenu()`.
- `renderControls()`: we use it to render controls page and show it by `displayControls()`.
- `renderAbout()`: we use it to render about page and show it by `displayAbout()`.

