#include "../include/Game.h"

/*
SDL_Texture* enemyBulletTexture = loadTexture(loadSurface("resources/enemyBullet.png"));
SDL_Texture* extraEnemyTexture = loadTexture(loadSurface("resources/extraEnemy.png"));
SDL_Texture* enemyFirstRowBTexture = loadTexture(loadSurface("resources/1b.png"));
SDL_Texture* enemySecondRowBTexture = loadTexture(loadSurface("resources/2b.png"));
SDL_Texture* enemyOtherRowsBTexture = loadTexture(loadSurface("resources/3b.png"));
SDL_Texture* enemyFirstRowATexture = loadTexture(loadSurface("resources/1a.png"));
SDL_Texture* enemySecondRowATexture = loadTexture(loadSurface("resources/2a.png"));
SDL_Texture* enemyOtherRowsATexture = loadTexture(loadSurface("resources/3a.png"));
SDL_Texture* deadEnemyTexture = loadTexture(loadSurface("resources/dead.png"));
*/

Game::Game(){
    //App app = mApp;
    initSDL();
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &app.window, &app.renderer);
    memset(&app, 0, sizeof(App));
    app.menu = true;
    //app.renderer = ren;
    smallSans = TTF_OpenFont("resources/virgo.ttf", 10);
    SansBold = TTF_OpenFont("resources/virgo.ttf", 50);
    
    playerLives = 3;

    memset(&player, 0, sizeof(Entity));
    
    std::cout << "bo" << std::endl;

    player.texture = loadTexture(loadSurface("resources/playership.png"));
    std::cout << "bo1" << std::endl;
    //SDL_Texture* playBackground = loadTexture(loadSurface("resources/background1.jpg"));
    enemyBulletTexture = loadTexture(loadSurface("resources/enemyBullet.png"));
    extraEnemyTexture = loadTexture(loadSurface("resources/extraEnemy.png"));
    enemyFirstRowBTexture = loadTexture(loadSurface("resources/1b.png"));
    enemySecondRowBTexture = loadTexture(loadSurface("resources/2b.png"));
    enemyOtherRowsBTexture = loadTexture(loadSurface("resources/3b.png"));
    std::cout << "bo2" << std::endl;
    enemyFirstRowATexture = loadTexture(loadSurface("resources/1a.png"));
    enemySecondRowATexture = loadTexture(loadSurface("resources/2a.png"));
    enemyOtherRowsATexture = loadTexture(loadSurface("resources/3a.png"));
    deadEnemyTexture = loadTexture(loadSurface("resources/dead.png"));

    menuImg = loadTexture(loadSurface("resources/background1.jpg"));
    std::cout << "bo3" << std::endl;
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
    
    std::cout << "boo" << std::endl;

    player.width = 100;
    player.height = 50;
    player.health = 1;
    //char scoreBuffer[20];
    scorePosition = {20, 10, 180, 50};
    prevScore = 0;

    spawnEnemies(&firstEnemy, &addedEnemy);
    spawningWalls(&walls);
    timeToSeeAddedScores = 1;

    player.reload = PLAYER_RELOAD;

    player.x = SCREEN_WIDTH / 2 - player.width / 2;
    player.y = 600;
    requestClose = 0;
    gameRunning = true;

    gameMenu();

    std::cout << "booo" << std::endl;
};

Game::~Game(){
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
};

void Game::gameMenu(){
    start = clock();                            //// LOOKUP
    prepareScene();
    blit(menuImg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    presentScene();
    std::cout << "boooo" << std::endl;
    while(app.menu){
        doInput(&requestClose);

    }

    gameloop();
}


void Game::gameloop(){
    while (!requestClose) {
        std::cout << "bo0000" << std::endl;
        //start = clock();                            //// LOOKUP
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
}