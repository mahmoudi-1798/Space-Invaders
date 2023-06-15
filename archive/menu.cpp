#include "../include/menu.h"

App menuApp;


Menu::Menu(SDL_Renderer* render, SDL_Window* window) {
    win = window;
    ren = render;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Failed at SDL_Init(), error: " << SDL_GetError() << endl;
    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &win, &ren) < 0) cout << "Failed at SDL_CreateWindowAndRenderer(), error: " << SDL_GetError() << endl;
    SDL_SetWindowTitle(win, "Our First Game!!!");
    TTF_Init();
    running=true;
    font = TTF_OpenFont("resources/Polsku.ttf", 25);
    
    

    gameStart = false;

    loop();
}

Menu::~Menu() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
}

void Menu::loop() {
    static int lastTime = 0;
    while(running) {
        lastFrame=SDL_GetTicks();
        if(lastFrame >= (lastTime+1000)) {
        lastTime=lastFrame;
        frameCount=0;
        }
        
        render();
        input();
        update();
    }
}

void Menu::render() {
    SDL_SetRenderDrawColor(ren, 126, 192, 238, 255);
    SDL_Rect rect;
    rect.x=rect.y=0;
    rect.w=SCREEN_WIDTH;
    rect.h=SCREEN_HEIGHT;
    SDL_RenderFillRect(ren, &rect);


    draw("Menu", 200, 200, 255, 255, 255);

    frameCount++;
    int timerFPS = SDL_GetTicks()-lastFrame;
    if(timerFPS<(1000/60)) {
        SDL_Delay((1000/60)-timerFPS);
    }

    SDL_RenderPresent(ren);
}


void Menu::draw(const char* msg, int x, int y, int r, int g, int b) {
    SDL_Surface* surf;
    SDL_Texture* tex;
    SDL_Color color;
    color.r=r;
    color.g=g;
    color.b=b;
    color.a=255;
    SDL_Rect rect;
    surf = TTF_RenderText_Solid(font, msg, color);
    tex = SDL_CreateTextureFromSurface(ren, surf);
    rect.x=x;
    rect.y=y;
    rect.w=300;
    rect.h=300;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(ren, tex, NULL, &rect);
    SDL_DestroyTexture(tex);
}

void Menu::input() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {running=false; cout << "Quitting" << endl;}
        if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_ESCAPE) running=false;
            if(e.key.keysym.sym == SDLK_e) {gameStart = true; running = false; Menu::~Menu();}
        }
        if(e.type == SDL_KEYUP) {
            //if(e.key.keysym.sym == SDLK_e) {l = 0; player.setCurAnimation(idoll);}

        }
    }
}


void Menu::update() {
    if(gameStart){getExit();}
    
}


bool Menu::getExit(){
    return true;
}

/*
std::string menuLoop(){
    memset(&app, 0, sizeof(App));
    //initSDL();
    //TTF_Font* smallSans = TTF_OpenFont("resources/Polsku.ttf", 10);
    //TTF_Font* SansBold = TTF_OpenFont("resources/Polsku.ttf", 50);


    //SDL_Texture* m_text1 = (SDL_Texture*) downloadingText(SansBold, "Menu");
    //SDL_Texture* m_text2 = (SDL_Texture*) downloadingText(SansBold, "press E to start game.");

    prepareScene();
    SDL_Rect coordinates;
    coordinates.w = 200;
    coordinates.h = 50;
    coordinates.x = SCREEN_WIDTH / 2 - coordinates.w / 2;
    coordinates.y = SCREEN_HEIGHT / 2 - (coordinates.h / 2) - 20;
    blit(m_text1, 100, 200, 200, 300);
    blit(m_text2, 100, 500, 200, 300);


    presentScene();

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        //if(e.type == SDL_QUIT) {running=false; cout << "Quitting" << endl;}
        if(e.type == SDL_KEYDOWN) {
            //if(e.key.keysym.sym == SDLK_ESCAPE) running=false;
            if(e.key.keysym.sym == SDLK_e) {return "e"; std::cout << "e pressed" << std::endl;}

        }
        //if(e.type == SDL_KEYUP) {
            //if(e.key.keysym.sym == SDLK_a) {}
        //}

    }


    SDL_DestroyTexture(m_text1);
    SDL_DestroyTexture(m_text2);
    SDL_DestroyRenderer(menuApp.renderer);
    SDL_DestroyWindow(menuApp.window);
    TTF_CloseFont(smallSans);
    TTF_CloseFont(SansBold);
    TTF_Quit();
    SDL_Quit();

    return "n";
}
*/