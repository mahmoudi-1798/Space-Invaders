#ifndef GAME_H_2
#define GAME_H_2

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <vector>

#include "common.h"
#include "structs.h"
#include "draw.h"
#include "input.h"
#include "init.h"
#include "defs.h"
#include "handlingPlayer.h"
#include "enemies.h"
#include "underLine.h"
#include "walls.h"
#include "scenes.h"

using namespace std;

#define WIDTH 1280
#define HEIGHT 720

class Game {
    public:
        Game();
        ~Game();
        void gameloop();
        void gameMenu();

        App app;
        Entity player;

        //SDL_Renderer* ren;
        //SDL_Window* win;
        TTF_Font* smallSans;
        TTF_Font* SansBold;
        bool running;

        SDL_Texture* enemyBulletTexture;
        SDL_Texture* extraEnemyTexture;
        SDL_Texture* enemyFirstRowBTexture;
        SDL_Texture* enemySecondRowBTexture;
        SDL_Texture* enemyOtherRowsBTexture;
        SDL_Texture* enemyFirstRowATexture;
        SDL_Texture* enemySecondRowATexture;
        SDL_Texture* enemyOtherRowsATexture;
        SDL_Texture* deadEnemyTexture;



        bool gameRunning;

        clock_t start, end;
        long timeElapsed;
        int timeToSeeAddedScores;
        int requestClose;    
        int prevScore;
        char scoreBuffer[20];
        SDL_Rect scorePosition;
        int playerLives;

        Entity* firstBullet;
        Entity* addedBullet;
        Entity* firstEnemy;
        Entity* addedEnemy;
        Entity* firstEnemyBullet;
        Entity* addedEnemyBullet;
        Entity* walls;
        Entity* extraEnemy;
        Interval* underLineTearIntervals;
        Interval* addedUnderLineTearInterval;


        SDL_Texture* menuImg;

        int init;

    private:
};



#endif