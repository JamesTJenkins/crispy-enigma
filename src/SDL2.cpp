#include "SDL2.h"
#include <iostream>

namespace SDLModule {
    SDL2::SDL2() {

    }

    SDL2::~SDL2() {
        
    }

    void SDL2::InitSDL2() {
        // Initalizes SLD2
        SDL_Init(SDL_INIT_VIDEO);
    }

    int SDL2::CreateWindow(const char *title, int x, int y, int width, int height, Uint32 flags) {
        // Creates a application window
        window = SDL_CreateWindow(
            title,      // Window title
            x,          // Inital x position
            y,          // Inital y position
            width,      // Window width
            height,     // Window height
            flags       // Window flags
        );

        // Check window was created successfully 
        if (window == NULL) {
            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            return 1;
        }

        return 0;
    }

    void SDL2::Cleanup() {
        SDL_DestroyWindow(window);
        SDL_Quit();

        std::cout << "SDL2 cleaned up" << std::endl;
    }
}