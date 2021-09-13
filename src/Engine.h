#pragma once
#include "Root.h"

namespace Engine {
	
	class Game;
	
	class Engine {
	public:
		Engine();
		~Engine();

		void Run(const Game& game);
		void Shutdown();

		Root root;
	private:
		void Init();
		void Cleanup();

		bool running = false;
	};
}