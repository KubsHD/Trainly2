#include "Game.h"

using namespace DirectX::SimpleMath;

Vector3 trainPos;
Pipeline unlitPipeline;

float counter = 0;

float updateFpsTimer = 0;

float fps = 0;
float deltaTime = 0;

std::vector<STRB::Ref<Model>> chunkQueue;


void Game::Init()
{
	// init sdl

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	// create window

	m_window = SDL_CreateWindow("Trainly 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	if (!m_window)
	{
		printf("SDL could not create window! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_window, &wmInfo);

	m_graphics = STRB::CreateRef<Graphics>();

	m_graphics->Init(wmInfo.info.win.window, m_window);

	content.Init("data", *m_graphics.get());

	debug = content.LoadFont("font/comic");
	train = content.LoadModel("model/train/mdl_train");

	unlitPipeline = {
		.vShader = content.LoadVertexShader("shader/unlit_vertex"),
		.pShader = content.LoadPixelShader("shader/unlit_pixel"),
	};

	float aspectRatio = 16.0 / 9.0f;
	cam = STRB::CreateRef<Camera>();
	cam->Init(60.0f, aspectRatio, 10.0f, 100000.0f);
	cam->SetPosition({ 1000.0f, 1000.0f, 0.0f});
	m_graphics->SetActiveCamera(cam);

	// load chunks

	auto chunk_paths = StringConverter::ReadAllLines("data/db/chunk.txt");

	for (auto& path : chunk_paths)
		m_chunks.push_back(content.LoadModel(path));

	for (int i = 0; i < 300; i++)
	{
		int randomNumber = rand() % 2;
		chunkQueue.push_back(m_chunks[randomNumber]);
	}

}


void Game::Update(float deltaTime)
{
	counter += Time.DeltaTime();

	trainPos.z += counter;

	float cameraSpeed = 10.0f;

	if (Input.IsKeyPressed(SDLK_LSHIFT))
	{
		cam->AppendPosition(cam->GetUpVector() * cameraSpeed);
	}

	if (Input.IsKeyPressed(SDLK_LCTRL))
	{
		cam->AppendPosition(cam->GetDownVector() * cameraSpeed);
	}

	if (Input.IsKeyPressed(SDLK_w))
	{
		cam->AppendPosition(cam->GetForwardVector() * cameraSpeed);
	}

	if (Input.IsKeyPressed(SDLK_s))
	{
		cam->AppendPosition(cam->GetBackwardVector() * cameraSpeed);
	}

	if (Input.IsKeyPressed(SDLK_a))
	{
		cam->AppendPosition(cam->GetLeftVector() * cameraSpeed);
	}

	if (Input.IsKeyPressed(SDLK_d))
	{
		cam->AppendPosition(cam->GetRightVector() * cameraSpeed);
	}

	//cam->SetPosition(Vector3(100.0f, 1000.0f, 0.0f) + trainPos);
	//cam->SetLookTargetPosition(trainPos);

	if (Input.IsMouseButtonPressed(SDL_BUTTON_RIGHT))
		cam->AppendRotation({ (Input.GetDeltaMousePos().y * 0.005f), (Input.GetDeltaMousePos().x * 0.005f), 0.0f });

	cam->SetPosition({ 1000.0f, 1000.0f, trainPos.z + 1000.0f });
	cam->SetRotation({ -100.0f, 180.0f, 0.0f });
}


void Game::Draw(STRB::Ref<Graphics> renderer)
{
	renderer->Clear(37, 121, 231);

	updateFpsTimer += Time.DeltaTime();

	renderer->BindPipeline(unlitPipeline);

	for (int i = 0; i < chunkQueue.size(); i++)
	{
		renderer->DrawModel(*chunkQueue[i], { 0.0f, 0.0f, 7200.0f * i });
	}


	
	renderer->DrawModel(*train, trainPos);

	if (updateFpsTimer >= 0.3)
	{
		fps = 1.0f / Time.DeltaTime();
		deltaTime = Time.DeltaTime();
		updateFpsTimer = 0;
	}

	renderer->DrawString("FPS: " + std::to_string(fps), { 10.0f, 0.0f }, *debug);
	renderer->DrawString("Delta: " + std::to_string(deltaTime), { 10.0f, 35.0f }, *debug);

	renderer->Present();

}

void Game::Clean()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::Run()
{
	Init();

	bool running = true;

	while (running)
	{
		SDL_PumpEvents();

		Time.Tick();

		Draw(m_graphics);

		Update(Time.DeltaTime());

		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				Input.SetKeyState(e.key.keysym.sym, true);
				break;
			case SDL_KEYUP:
				Input.SetKeyState(e.key.keysym.sym, false);
				break;
			case SDL_MOUSEBUTTONDOWN:
				Input.SetMouseButtonState(e.button.button, true);
				break;
			case SDL_MOUSEBUTTONUP:
				Input.SetMouseButtonState(e.button.button, false);
				break;
			default:
				break;
			}
		}

		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		Input.SetDeltaMouseState({ (float)x , (float)y });

		SDL_GetMouseState(&x, &y);
		Input.SetMousePosition({ (float)x , (float)y });
	}


	Clean();
}


