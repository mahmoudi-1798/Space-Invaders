#include "include/common.h"
#include "include/structs.h"
#include "include/defs.h"
#include "include/draw.h"
#include "include/handlingPlayer.h"
#include "include/input.h"

void showTransitionalScene(int playerLives, TTF_Font* SansBold) {
    char buffer[50];
    snprintf(buffer, 50, "%d Lives left", playerLives);
    
    SDL_Rect Message_rect;
    Message_rect.x = SCREEN_WIDTH;
    Message_rect.h = 100;
    Message_rect.w = 300;
    Message_rect.y = SCREEN_HEIGHT / 2 - Message_rect.h / 2;

    SDL_Texture* looseMessage = (SDL_Texture*) downloadingText(SansBold, buffer);
    SDL_Texture* looseBackground = loadTexture(loadSurface("resources/background1.jpg"));

    int closeWindowInAfterLooseScene = 0;

    while (Message_rect.x + Message_rect.w >= 0 && !closeWindowInAfterLooseScene) {
        prepareScene();
        blit(looseBackground, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    closeWindowInAfterLooseScene = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
			            closeWindowInAfterLooseScene = 1;
		            }
                    break;
                default:
                    break;
            }
        }

        Message_rect.x -= 8;
        SDL_RenderCopy(app.renderer, looseMessage, NULL, &Message_rect);
        presentScene();
        SDL_Delay(1000/60);
    }

    SDL_DestroyTexture(looseMessage);
}

void showGameOverScene(TTF_Font* SansBold, TTF_Font* smallSans) {
    SDL_Texture* gameOverBackground = loadTexture(loadSurface("resources/you_died.png"));

    prepareScene();
    SDL_Rect coordinates;
    coordinates.w = 200;
    coordinates.h = 50;
    coordinates.x = SCREEN_WIDTH / 2 - coordinates.w / 2;
    coordinates.y = SCREEN_HEIGHT / 2 - (coordinates.h / 2) - 20;
    blit(gameOverBackground, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    presentScene();
    SDL_Delay(5000);

    SDL_DestroyTexture(gameOverBackground);
}