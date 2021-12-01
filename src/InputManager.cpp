#include "InputManager.h"
#include <iostream>

namespace Manager {
    InputManager::InputManager() {
        SDL_GetMouseState(&mouseX, &mouseY);
    }

    InputManager::~InputManager() {

    }

    void InputManager::HandleKeyboardInput() {
        keystates = SDL_GetKeyboardState(NULL);
    }

    void InputManager::HandleMouseInput() {
        SDL_GetMouseState(&mouseX, &mouseY);
    }

    int InputManager::GetMouseX() {
        return mouseX;
    }

    int InputManager::GetMouseY() {
        return mouseY;
    }

    void InputManager::GetMousePosition(int *x, int *y) {
        *x = GetMouseX();
        *y = GetMouseY();
    }

    bool InputManager::GetKeyState(SDL_Scancode keyScancode) {
        return keystates[keyScancode];
    }
}