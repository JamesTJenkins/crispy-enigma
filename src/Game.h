#pragma once
#include "Engine.h"

namespace Engine {
	class Game {
		friend Engine;
	public:
		virtual ~Game() = 0;

		virtual void Start() = 0;
		virtual void Update() = 0;
	protected:
		Engine* GetEngine();
	private:
		Engine* engine = nullptr;
	};
}