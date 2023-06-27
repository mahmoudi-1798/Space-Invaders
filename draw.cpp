#include "include/common.h"
#include "include/base.h"

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

SDL_Surface *loadSurface(const char *path)
{
    SDL_Surface *surface = IMG_Load(path);

    if (!surface)
    {
        std::cout << "Error creating a surface" << std::endl;
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    return surface;
}

SDL_Texture *loadTexture(SDL_Surface *surface)
{
    SDL_Texture *tex = SDL_CreateTextureFromSurface(app.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        std::cout << "Error creating a texture"<< SDL_GetError() << std::endl;
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    return tex;
}

void blit(SDL_Texture *texture, int x, int y, int w, int h)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    dest.w = w;
    dest.h = h;

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}



void setPixelInASurface(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, int depth) {

    if(x < surface->w  && y < surface->h && x >= 0 && y >= 0) {
        SDL_LockSurface(surface);
        uint8_t* pixelArray = (uint8_t*) surface->pixels;
        int neededPixel = y*surface->pitch + x*surface->format->BytesPerPixel;

        pixelArray[neededPixel] = g;
        pixelArray[neededPixel + 1] = b;
        pixelArray[neededPixel + 2] = r;

        SDL_UnlockSurface(surface);
    }
}
