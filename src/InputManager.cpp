#include "InputManager.h"
#include <iostream>

namespace Manager {
    InputManager::InputManager() {

    }

    InputManager::~InputManager() {

    }

    void InputManager::HandleKeyboardInput() {
        keystates = SDL_GetKeyboardState(NULL);
    }

    bool InputManager::GetKeyState(SDL_Scancode keyScancode) {
        return keystates[keyScancode];
    }
}