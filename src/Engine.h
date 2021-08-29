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

		void Draw();

		Root root;
	private:
		void Init();
		void Cleanup();

		void LoadData();
		void InitVulkan();

		bool running = false;
	};
}