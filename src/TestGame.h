#pragma once
#include "Game.h"

namespace Game {
	class TestGame : public Engine::Game {
	public:
		TestGame();
		~TestGame();

		void Start();
		void Update();
	};
}