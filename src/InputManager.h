#pragma once 
#include <SDL2/SDL.h>
#include <map>

namespace Manager {
    class InputManager {
    public:
        InputManager();
        ~InputManager();

        void HandleKeyboardInput();
        bool GetKeyState(SDL_Scancode keyScancode);
    private:
        const Uint8* keystates;
    };
}