#include "../include/game.h"

SDL_Texture* enemyBulletTexture;
SDL_Texture* extraEnemyTexture;
SDL_Texture* enemyFirstRowBTexture;
SDL_Texture* enemySecondRowBTexture;
SDL_Texture* enemyOtherRowsBTexture;
SDL_Texture* enemyFirstRowATexture;
SDL_Texture* enemySecondRowATexture;
SDL_Texture* enemyOtherRowsATexture;
SDL_Texture* deadEnemyTexture;

App app;
Entity player;

bool running;

void menuLoop(){}

void gameLoop(){
    //App app = mApp;
    memset(&app, 0, sizeof(App));
    initSDL();
    TTF_Font* smallSans = TTF_OpenFont("resources/virgo.ttf", 10);
    TTF_Font* SansBold = TTF_OpenFont("resources/virgo.ttf", 50);
    
    int playerLives = 3;

    memset(&player, 0, sizeof(Entity));

    player.texture = loadTexture(loadSurface("resources/playership.png"));
    //SDL_Texture* playBackground = loadTexture(loadSurface("resources/background1.jpg"));
    enemyBulletTexture = loadTexture(loadSurface("resources/enemyBullet.png"));
    extraEnemyTexture = loadTexture(loadSurface("resources/extraEnemy.png"));
    enemyFirstRowBTexture = loadTexture(loadSurface("resources/1b.png"));
    enemySecondRowBTexture = loadTexture(loadSurface("resources/2b.png"));
    enemyOtherRowsBTexture = loadTexture(loadSurface("resources/3b.png"));
    enemyFirstRowATexture = loadTexture(loadSurface("resources/1a.png"));
    enemySecondRowATexture = loadTexture(loadSurface("resources/2a.png"));
    enemyOtherRowsATexture = loadTexture(loadSurface("resources/3a.png"));
    deadEnemyTexture = loadTexture(loadSurface("resources/dead.png"));


    Entity* firstBullet = NULL;
    Entity* addedBullet = NULL;
    Entity* firstEnemy = NULL;
    Entity* addedEnemy = NULL;
    Entity* firstEnemyBullet = NULL;
    Entity* addedEnemyBullet = NULL;
    Entity* walls = NULL;
    Entity* extraEnemy = makeExtraEnemy();
    Interval* underLineTearIntervals = NULL;
    Interval* addedUnderLineTearInterval = underLineTearIntervals;
    
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
        // blit(playBackground, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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
            snprintf(scoreBuffer, 20, "Score: %d +%d", player.score, player.score - prevScore);
            timeToSeeAddedScores = 120;
        } else if (--timeToSeeAddedScores == 0) {
            ++timeToSeeAddedScores;
            snprintf(scoreBuffer, 20, "Score: %d", player.score);
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
        // printf("Time elapsed - %ld\n", timeElapsed);

        if (!player.health && --playerLives > 0) {
            //showTransitionalScene(playerLives, SansBold);
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
    TTF_Quit();
    SDL_Quit();
}