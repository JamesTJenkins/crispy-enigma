#define SDL_MAIN_HANDLED

#include "Cramgine.h"
#include "Engine.h"
#include "TestGame.h"

int main() {
	std::cout << "Cramgine Initalizing..." << std::endl;

	Engine::Engine engine;
	Game::TestGame game;

	engine.Run(game);

	return 0;
}