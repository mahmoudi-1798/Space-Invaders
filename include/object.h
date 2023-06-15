#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Object{
    private:
        SDL_Rect dest;
        SDL_Rect src;
        SDL_Texture* tex;
        bool solid;
        int id;
    public:
        Object() {solid=1;};
        SDL_Rect getDest() const {return dest;}
        SDL_Rect getSource() const {return src;}
        SDL_Texture* getTex() const {return tex;}
        void setDest(int x, int y, int w, int h);
        void setDest(int x, int y);
        void setSource(int x, int y, int w, int h);
        void setImage(std::string fileName, SDL_Renderer* ren);
        void setSolid(bool s) {solid=s;}
        bool getSolid() const {return solid;}
        int getDX() const {return dest.x;};
        int getDY() const {return dest.y;};
        int getDW() const {return dest.w;};
        int getDH() const {return dest.h;};
        void setId(int i) {id=i;}
        int getId() {return id;}
};



#endif