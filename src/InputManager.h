#pragma once 
#include <SDL2/SDL.h>
#include <map>

namespace Manager {
    class InputManager {
    public:
        InputManager();
        ~InputManager();

        // Updates mouse position and keyboard inputs
        
        void HandleKeyboardInput();
        void HandleMouseInput();

        // Mouse Functions

        int GetMouseX();
        int GetMouseY();
        void GetMousePosition(int *x, int *y);

        // Keyboard Functions

        bool GetKeyState(SDL_Scancode keyScancode);
    private:
        const Uint8* keystates;
        
        int mouseX;
        int mouseY;
    };
}