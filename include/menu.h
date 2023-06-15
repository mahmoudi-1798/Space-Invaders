#ifndef MENU_H
#define MENU_H

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

//bool menuExit;

//std::string menuLoop();

using namespace std;

class Menu {
    public:
        Menu(SDL_Renderer* render, SDL_Window* window);
        ~Menu();
        void loop();
        void update();
        void input();
        void render();
        void draw(const char* msg, int x, int y, int r, int g, int b);
        bool getExit();

    private:
        SDL_Renderer* ren;
        SDL_Window* win;
        TTF_Font *font;
        bool running;
        int count;
        int frameCount, timerFPS, lastFrame;

        bool gameStart;
};




#endif