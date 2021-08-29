#pragma once
#include <SDL2/SDL.h>

namespace SDLModule {
    class SDL2 {
    public:
        SDL2();
        ~SDL2();

        void InitSDL2();
        int CreateWindow(const char *title, int x, int y, int width, int height, Uint32 flags);
        void Cleanup();

        int width = 1280;
        int height = 720;
        SDL_Window *window;
    };
}