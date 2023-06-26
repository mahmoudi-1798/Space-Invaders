
#include "include/common.h"
#include "include/structs.h"
#include "include/draw.h"
#include "include/input.h"
#include "include/init.h"
#include "include/defs.h"
#include "include/handlingPlayer.h"
#include "include/enemies.h"
#include "include/underLine.h"
#include "include/walls.h"
#include "include/scenes.h"

#define SMALL_SANS 25
#define BOLD_SANS 40
#define BIG_SANS 120

SDL_Texture* enemyBulletTexture;
SDL_Texture* extraEnemyTexture;
SDL_Texture* enemyFirstRowBTexture;
SDL_Texture* enemySecondRowBTexture;
SDL_Texture* enemyOtherRowsBTexture;
SDL_Texture* enemyFirstRowATexture;
SDL_Texture* enemySecondRowATexture;
SDL_Texture* enemyOtherRowsATexture;
SDL_Texture* deadEnemyTexture;

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

App app;
Entity player;
Entity cursor;

void renderControls(SDL_Renderer* renderer, TTF_Font* font1, TTF_Font* font2);
void displayMenu(SDL_Window* window, SDL_Renderer* renderer, bool &game, bool &menu, bool& controls, bool& about, bool &fullscreen,int &menuOption, SDL_Event e);
void renderControls(SDL_Renderer* renderer, TTF_Font* font1, TTF_Font* font2);
void displayControls(SDL_Renderer* renderer, bool &controls, bool &menu, SDL_Event e);
void renderAbout(SDL_Renderer* renderer, TTF_Font* font1, TTF_Font* font2);
void displayAbout(SDL_Renderer* renderer, bool &about, bool &menu, SDL_Event e);

bool fullscreen = false;

int main() {
    // basic initialization
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    memset(&app, 0, sizeof(App));

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    app.renderer = renderer;
    app.window = window;
    app.fullscreen = false;

    TTF_Font* smallSans = TTF_OpenFont("resources/virgo.ttf", SMALL_SANS);
    TTF_Font* SansBold = TTF_OpenFont("resources/EnvyCode.ttf", BOLD_SANS);
    TTF_Font* SansBig = TTF_OpenFont("resources/EnvyCode.ttf", BIG_SANS);

    bool menu = true;
    bool game = false;
    bool controls;
    bool about;
    SDL_Event e;
    int menuOption = 0;
    
    // displaying menu
    displayMenu(window ,renderer, game, menu, controls, about, fullscreen, menuOption, e);

    // main game loop
    while(game){
        int playerLives = 3;

        memset(&player, 0, sizeof(Entity));

        player.texture = loadTexture(loadSurface("resources/player.png"));
        enemyBulletTexture = loadTexture(loadSurface("resources/enemyBullet.png"));
        extraEnemyTexture = loadTexture(loadSurface("resources/extraEnemy.png"));
        enemyFirstRowBTexture = loadTexture(loadSurface("resources/1b.png"));
        enemySecondRowBTexture = loadTexture(loadSurface("resources/2b.png"));
        enemyOtherRowsBTexture = loadTexture(loadSurface("resources/3b.png"));
        enemyFirstRowATexture = loadTexture(loadSurface("resources/1a.png"));
        enemySecondRowATexture = loadTexture(loadSurface("resources/2a.png"));
        enemyOtherRowsATexture = loadTexture(loadSurface("resources/3a.png"));
        deadEnemyTexture = loadTexture(loadSurface("resources/dead.png"));


        firstBullet = NULL;
        addedBullet = NULL;
        firstEnemy = NULL;
        addedEnemy = NULL;
        firstEnemyBullet = NULL;
        addedEnemyBullet = NULL;
        walls = NULL;
        extraEnemy = makeExtraEnemy();
        underLineTearIntervals = NULL;
        addedUnderLineTearInterval = underLineTearIntervals;
        
        player.width = 100;
        player.height = 50;
        player.health = 1;
        char scoreBuffer[20];
        SDL_Rect scorePosition = {20, 10, 180, 50};
        int prevScore = 0;

        spawnEnemies(&firstEnemy, &addedEnemy);
        spawningWalls(&walls);
        clock_t start, end;
        long timeElapsed;
        int timeToSeeAddedScores = 1;

        player.reload = PLAYER_RELOAD;

        player.x = SCREEN_WIDTH / 2 - player.width / 2;
        player.y = 600;
        int requestClose = 0;

        while (!requestClose) {

            start = clock();
            prepareScene();

            doInput(&requestClose);

            handlePlayer(&firstBullet, &addedBullet);

            handlingEnemy(&firstEnemy, &firstEnemyBullet, &addedEnemyBullet);

            SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
            
            drawBullets(&firstBullet, firstEnemy, &addedBullet, &firstEnemyBullet, &addedEnemyBullet,
            &underLineTearIntervals, &addedUnderLineTearInterval, walls, extraEnemy);
            if (player.reload != PLAYER_RELOAD) {
                player.reload++;
            }

            drawingUnderLine(underLineTearIntervals);

            blit(player.texture, player.x, player.y, player.width, player.height);
            spawnExtraEnemy(extraEnemy);

            if (prevScore != player.score) {
                snprintf(scoreBuffer, 20, "Score:  %d  +  %d", player.score, player.score - prevScore);
                timeToSeeAddedScores = 120;
            } else if (--timeToSeeAddedScores == 0) {
                ++timeToSeeAddedScores;
                snprintf(scoreBuffer, 20, "Score:  %d", player.score);
            }

            SDL_Texture* scoreTexture = (SDL_Texture*) downloadingText(SansBold, scoreBuffer);

            blit(scoreTexture, scorePosition.x, scorePosition.y, scorePosition.w, scorePosition.h);
            SDL_DestroyTexture(scoreTexture);
            presentingLivesInLow(smallSans, playerLives);

            drawingWalls(walls);

            wallBoom(walls);

            end = clock();

            timeElapsed = timediff(start, end);

            presentScene();

            if (timeElapsed < (1000 / 60)) SDL_Delay(1000/60 - timeElapsed);

            if (!player.health && --playerLives > 0) {
                player.health = 1;
                player.score -= (100 / playerLives);
                app.left = 0;
                app.right = 0;
                freeingAllocations(firstBullet);
                freeingAllocations(firstEnemyBullet);
                firstBullet = NULL;
                addedBullet = NULL;
                firstEnemyBullet = NULL;
                addedEnemyBullet = NULL;
                timeToSeeAddedScores = 1;
                player.x = SCREEN_WIDTH / 2 - player.width / 2;
                player.y = 600;
            }

            if (!playerLives) {
                requestClose = 1;
            }

            prevScore = player.score;
        }

        if (!player.health && !playerLives) {
            showGameOverScene(SansBold, smallSans);
            game = false;
            menu = true;
            displayMenu(window, renderer, game, menu, controls, about, fullscreen, menuOption, e);
        }
    }

    SDL_DestroyTexture(enemyBulletTexture);
    SDL_DestroyTexture(extraEnemyTexture);
    SDL_DestroyTexture(enemyFirstRowBTexture);
    SDL_DestroyTexture(enemySecondRowBTexture);
    SDL_DestroyTexture(enemyOtherRowsBTexture);
    SDL_DestroyTexture(enemyFirstRowATexture);
    SDL_DestroyTexture(enemySecondRowATexture);
    SDL_DestroyTexture(enemyOtherRowsATexture);
    SDL_DestroyTexture(deadEnemyTexture);
    freeingAllocations(extraEnemy);
    freeingAllocations(walls);
    freeingAllocations(firstEnemyBullet);
    freeingAllocations(firstEnemy);
    freeingAllocations(firstBullet);
    while(underLineTearIntervals) {
        Interval* freee = underLineTearIntervals;
        underLineTearIntervals = underLineTearIntervals->next;
        free(freee);
    }
    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    TTF_CloseFont(smallSans);
    TTF_CloseFont(SansBold);
    TTF_CloseFont(SansBig);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}

// preparing & rendering menu 
void renderMenu(SDL_Renderer* renderer, TTF_Font* font1, TTF_Font* font2, int c_x, int c_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255 };

    SDL_Surface* logo = IMG_Load("resources/logo.png");
    SDL_Texture* logoTex = SDL_CreateTextureFromSurface(renderer, logo);
    SDL_Rect logoRect = {185, 30, 900, 389};
    SDL_RenderCopy(renderer, logoTex, NULL, &logoRect);

    memset(&cursor, 0, sizeof(Entity));
    cursor.texture = loadTexture(loadSurface("resources/cursor.png"));
    cursor.x = c_x;
    cursor.y = c_y;
    cursor.width = 17;
    cursor.height = 19;
    blit(cursor.texture, cursor.x, cursor.y, cursor.width, cursor.height);

    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font1, "New Game", textColor);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    
    SDL_Surface* textSurface3 = TTF_RenderText_Solid(font1, "Controls", textColor);
    SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
    
    SDL_Surface* textSurface4 = TTF_RenderText_Solid(font1, "About", textColor);
    SDL_Texture* textTexture4 = SDL_CreateTextureFromSurface(renderer, textSurface4);

    SDL_Surface* textSurface5 = TTF_RenderText_Solid(font1, "Exit", textColor);
    SDL_Texture* textTexture5 = SDL_CreateTextureFromSurface(renderer, textSurface5);

     
    SDL_Rect textRect2 = { SCREEN_WIDTH / 2 - textSurface2->w / 2, (SCREEN_HEIGHT / 2 - textSurface2->h / 2) + 140,
                          textSurface2->w, textSurface2->h };
    SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);
     
    SDL_Rect textRect3 = { SCREEN_WIDTH / 2 - textSurface3->w / 2, (SCREEN_HEIGHT / 2 - textSurface3->h / 2) + 190,
                          textSurface3->w, textSurface3->h };
    SDL_RenderCopy(renderer, textTexture3, NULL, &textRect3);

    SDL_Rect textRect4 = { SCREEN_WIDTH / 2 - textSurface4->w / 2, (SCREEN_HEIGHT / 2 - textSurface4->h / 2) + 240,
                          textSurface4->w, textSurface4->h };
    SDL_RenderCopy(renderer, textTexture4, NULL, &textRect4);

    SDL_Rect textRect5 = { SCREEN_WIDTH / 2 - textSurface5->w / 2, (SCREEN_HEIGHT / 2 - textSurface5->h / 2) + 290,
                          textSurface5->w, textSurface5->h };
    SDL_RenderCopy(renderer, textTexture5, NULL, &textRect5);

    SDL_DestroyTexture(cursor.texture);

    SDL_DestroyTexture(logoTex);
    SDL_FreeSurface(logo);

    SDL_DestroyTexture(textTexture2);
    SDL_FreeSurface(textSurface2);
    
    SDL_DestroyTexture(textTexture3);
    SDL_FreeSurface(textSurface3);

    SDL_DestroyTexture(textTexture4);
    SDL_FreeSurface(textSurface4);

    SDL_DestroyTexture(textTexture5);
    SDL_FreeSurface(textSurface5);

    SDL_RenderPresent(renderer);
}

// displaying menu
void displayMenu(SDL_Window* window, SDL_Renderer* renderer, bool &game, bool &menu, bool& controls, bool &about, bool &fullscreen,int &menuOption, SDL_Event e){

    TTF_Font* SansBold = TTF_OpenFont("resources/EnvyCode.ttf", BOLD_SANS);
    TTF_Font* SansBig = TTF_OpenFont("resources/EnvyCode.ttf", BIG_SANS);
    
    cursor.x = SCREEN_WIDTH / 2 - 120;
    cursor.y = SCREEN_HEIGHT/2 + 130;

    while(menu){
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                menu = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP) {
                    // Handle up arrow key press to move the selection up
                    menuOption = (menuOption - 1 + 4) % 4; 
                    if (menuOption == 3) {cursor.y = SCREEN_HEIGHT/2 + 280;}
                    else {cursor.y -= 50;}
                    
                }
                else if (e.key.keysym.sym == SDLK_DOWN) {
                    // Handle down arrow key press to move the selection down
                    menuOption = (menuOption + 1) % 4;
                    if (menuOption == 0) {cursor.y = SCREEN_HEIGHT/2 + 130;}
                    else {cursor.y += 50;}
                    
                }
                else if (e.key.keysym.sym == SDLK_RETURN) {
                    // Handle enter key press to select an option
                    switch (menuOption) {
                        case 0:
                            // Start game
                            menu = false; // Exit the menu loop
                            game = true;
                            break;
                        case 1:
                            // Options
                            controls = true;
                            displayControls(renderer, controls, menu, e);
                            break;
                        case 2:
                            // About game
                            about = true;
                            displayAbout(renderer, about, menu, e);
                            break;

                        case 3:
                            // Exit game
                            menu = false; 
                            game = false;
                            break;
                    }
                }
                else if(e.key.keysym.sym == SDLK_F11){
                    if(fullscreen){
                        SDL_SetWindowFullscreen(window, 0);
                    }
                    else{
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                        SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
                    }  
                }
            }
        }
        renderMenu(renderer, SansBold, SansBig, cursor.x, cursor.y);
    }
}

// preparing & rendering controls 
void renderControls(SDL_Renderer* renderer, TTF_Font* font1, TTF_Font* font2){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font2, "Controls", textColor);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);

    SDL_Surface* lineSurface = IMG_Load("resources/line2.png");
    SDL_Texture* lineTex = SDL_CreateTextureFromSurface(renderer, lineSurface);
    SDL_Rect lineRect = {SCREEN_WIDTH / 2 - lineSurface->w / 2, (SCREEN_HEIGHT / 2 - lineSurface->h / 1) -140,
                          lineSurface->w, lineSurface->h};
    SDL_RenderCopy(renderer, lineTex, NULL, &lineRect);
    
    SDL_Surface* textSurface31 = TTF_RenderText_Solid(font1, "Right:                D Key", textColor);
    SDL_Texture* textTexture31= SDL_CreateTextureFromSurface(renderer, textSurface31);
    
    SDL_Surface* textSurface32 = TTF_RenderText_Solid(font1, "Left:                 A key", textColor);
    SDL_Texture* textTexture32 = SDL_CreateTextureFromSurface(renderer, textSurface32);

    SDL_Surface* textSurface4 = TTF_RenderText_Solid(font1, "Shoot:                SPACE key", textColor);
    SDL_Texture* textTexture4 = SDL_CreateTextureFromSurface(renderer, textSurface4);

    SDL_Surface* textSurface5 = TTF_RenderText_Solid(font1, "Fullscreen:           F11", textColor);
    SDL_Texture* textTexture5 = SDL_CreateTextureFromSurface(renderer, textSurface5);
    
    SDL_Rect textRect1 = { SCREEN_WIDTH / 2 - textSurface1->w / 2, (SCREEN_HEIGHT / 2 - textSurface1->h / 1) -170,
                          textSurface1->w, textSurface1->h };
    SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
     
    SDL_Rect textRect31 = { 450, 270,
                          textSurface31->w, textSurface31->h };
    SDL_RenderCopy(renderer, textTexture31, NULL, &textRect31);

    SDL_Rect textRect32 = {450, 320,
                          textSurface32->w, textSurface32->h };
    SDL_RenderCopy(renderer, textTexture32, NULL, &textRect32);

    SDL_Rect textRect4 = { 450, 370,
                          textSurface4->w, textSurface4->h };
    SDL_RenderCopy(renderer, textTexture4, NULL, &textRect4);

    SDL_Rect textRect5 = { 450, 420,
                          textSurface5->w, textSurface5->h };
    SDL_RenderCopy(renderer, textTexture5, NULL, &textRect5);

    SDL_DestroyTexture(textTexture1);
    SDL_FreeSurface(textSurface1);

    SDL_DestroyTexture(lineTex);
    SDL_FreeSurface(lineSurface);

    SDL_DestroyTexture(textTexture31);
    SDL_FreeSurface(textSurface31);

    SDL_DestroyTexture(textTexture32);
    SDL_FreeSurface(textSurface32);

    SDL_DestroyTexture(textTexture4);
    SDL_FreeSurface(textSurface4);

    SDL_DestroyTexture(textTexture5);
    SDL_FreeSurface(textSurface5);

    SDL_RenderPresent(renderer);
}

// displaying controls
void displayControls(SDL_Renderer* renderer, bool &controls, bool &menu, SDL_Event e){
    TTF_Font* SansBold = TTF_OpenFont("resources/EnvyCode.ttf", BOLD_SANS);
    TTF_Font* SansSmall = TTF_OpenFont("resources/EnvyCode.ttf", SMALL_SANS);
    while(controls){
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                controls = false;
                menu = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    controls = false;
                    menu = true;
                }
            }
        }
        renderControls(renderer, SansSmall, SansBold);
    }
       
}

// preparing & rendering about 
void renderAbout(SDL_Renderer* renderer, TTF_Font* font1, TTF_Font* font2){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font2, "About", textColor);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);

    SDL_Surface* lineSurface = IMG_Load("resources/line2.png");
    SDL_Texture* lineTex = SDL_CreateTextureFromSurface(renderer, lineSurface);
    SDL_Rect lineRect1 = {SCREEN_WIDTH / 2 - lineSurface->w / 2, (SCREEN_HEIGHT / 2 - lineSurface->h / 1) -170,
                          lineSurface->w, lineSurface->h};
    SDL_Rect lineRect2 = {SCREEN_WIDTH / 2 - lineSurface->w / 2, (SCREEN_HEIGHT / 2 - lineSurface->h / 11) + 60,
                          lineSurface->w, lineSurface->h};
    SDL_RenderCopy(renderer, lineTex, NULL, &lineRect1);
    SDL_RenderCopy(renderer, lineTex, NULL, &lineRect2);

    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font1, "Final project of Advanced Programming Course", textColor);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    
    SDL_Surface* textSurface11 = TTF_RenderText_Solid(font2, "Socials", textColor);
    SDL_Texture* textTexture11 = SDL_CreateTextureFromSurface(renderer, textSurface11);

    SDL_Surface* textSurface31 = TTF_RenderText_Solid(font1, "Github:           github.com/mahmoudi-1798", textColor);
    SDL_Texture* textTexture31= SDL_CreateTextureFromSurface(renderer, textSurface31);
    
    SDL_Surface* textSurface32 = TTF_RenderText_Solid(font1, "Linkedin:         linkedin.com/in/mohammad-hassan-mahmoudi", textColor);
    SDL_Texture* textTexture32 = SDL_CreateTextureFromSurface(renderer, textSurface32);
    
    SDL_Rect textRect1 = { SCREEN_WIDTH / 2 - textSurface1->w / 2, (SCREEN_HEIGHT / 2 - textSurface2->h / 1) -200,
                          textSurface1->w, textSurface1->h };
    SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);

    SDL_Rect textRect11 = { SCREEN_WIDTH / 2 - textSurface11->w / 2, (SCREEN_HEIGHT / 2 - textSurface2->h / 11)  ,
                          textSurface11->w, textSurface11->h };
    SDL_RenderCopy(renderer, textTexture11, NULL, &textRect11);     

    SDL_Rect textRect2 = { SCREEN_WIDTH / 2 - textSurface2->w / 2, (SCREEN_HEIGHT / 2 - textSurface2->h / 2) -130,
                          textSurface2->w, textSurface2->h };
    SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);
     
    SDL_Rect textRect31 = { 360, 450,
                          textSurface31->w, textSurface31->h };
    SDL_RenderCopy(renderer, textTexture31, NULL, &textRect31);

    SDL_Rect textRect32 = { 360, 500,
                          textSurface32->w, textSurface32->h };
    SDL_RenderCopy(renderer, textTexture32, NULL, &textRect32);

    SDL_DestroyTexture(textTexture1);
    SDL_FreeSurface(textSurface1);

    SDL_DestroyTexture(lineTex);
    SDL_FreeSurface(lineSurface);

    SDL_DestroyTexture(textTexture11);
    SDL_FreeSurface(textSurface11);

    SDL_DestroyTexture(textTexture2);
    SDL_FreeSurface(textSurface2);
    
    SDL_DestroyTexture(textTexture31);
    SDL_FreeSurface(textSurface31);

    SDL_DestroyTexture(textTexture32);
    SDL_FreeSurface(textSurface32);

    SDL_RenderPresent(renderer);
}

// displaying about
void displayAbout(SDL_Renderer* renderer, bool &about, bool &menu, SDL_Event e){
    TTF_Font* SansBold = TTF_OpenFont("resources/EnvyCode.ttf", BOLD_SANS);
    TTF_Font* SansSmall = TTF_OpenFont("resources/EnvyCode.ttf", SMALL_SANS);
    while(about){
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                about = false;
                menu = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    about = false;
                    menu = true;
                }
            }
        }
        renderAbout(renderer, SansSmall, SansBold);
    }     
}