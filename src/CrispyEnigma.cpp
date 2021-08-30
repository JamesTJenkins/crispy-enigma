#define SDL_MAIN_HANDLED
#include "CrispyEnigma.h"
#include "Engine.h"
#include "TestGame.h"

int main() {
	std::cout << "Crispy Enigma engine Initalizing..." << std::endl;

	Engine::Engine engine;
	Game::TestGame game;

	engine.Run(game);

	return 0;
}