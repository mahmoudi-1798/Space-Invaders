#include "include/common.h"
#include "include/base.h"

void doKeyDown(SDL_KeyboardEvent* event) {
    if (event->repeat == 0) {

        if (event->keysym.sym == SDLK_a) {
			app.left = 1;
		}

		if (event->keysym.sym == SDLK_d) {
			app.right = 1;
		}

        if (event->keysym.sym == SDLK_SPACE) {
            app.fire = 1;
        }

        if(event->keysym.sym == SDLK_p){ app.menu = false;}

        if(event->keysym.sym == SDLK_F11) { 
            if(app.fullscreen){
                SDL_SetWindowFullscreen(app.window, 0);
            }
            else{
                SDL_SetWindowFullscreen(app.window, SDL_WINDOW_FULLSCREEN);
            }            
        }

    }
}

void doKeyUp(SDL_KeyboardEvent* event) {
    if (event->repeat == 0) {

        if (event->keysym.sym == SDLK_a) {
			app.left = 0;
		}

		if (event->keysym.sym == SDLK_d) {
			app.right = 0;
		}

        if (event->keysym.sym == SDLK_SPACE) {
            app.fire = 0;
        }

    }
}

void doInput(int* requestClose) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				*requestClose = 1;
				break;
            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;
			default:
				break;
		}
	}
}