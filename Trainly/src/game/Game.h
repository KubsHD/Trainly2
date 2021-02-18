#pragma once

#include "../stdafx.h"
#include "../graphics/graphics.h"
#include "../timer.h"
#include "../InputManager.h"
#include "../io/ResourceManager.h"

class Game {
public:
	Game() {};

	void Run();

	void Init();

	void Update(Timer& time);
	void Draw(STRB::Ref<Graphics> renderer);

	void Clean();

	void Reload();

	InputManager Input;
	Timer Time;

private:
	SDL_Window* m_window;
	
	STRB::Ref<Graphics> m_graphics;

	ResourceManager content;

	std::vector<STRB::Ref<Model>> m_chunks;

	STRB::Ref<Model> train;
	STRB::Ref<Model> rail;

	STRB::Ref<Font> debug;

	STRB::Ref<Camera> cam;
};