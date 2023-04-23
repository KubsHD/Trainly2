#include "Game.h"


using namespace DirectX::SimpleMath;

#pragma region GameVariables


float trainSpeed = 200;

Vector3 trainPos;
Pipeline unlitPipeline;

float counter = 0;

float updateFpsTimer = 0;

float fps = 0;
float deltaTime = 0;

STRB::Ref<Model> railModel;

Vector3 railRootPos = { 0.0f, 0.0f, 600.0f };

int currentRail = 0;

struct RailObject
{
	STRB::Ref<Model> railModel;
	Vector3 pos;
	Quaternion rot;
};

bool godMode = false;

std::vector<STRB::Ref<Model>> chunkQueue;
std::vector<RailObject> railQueue;

Pipeline litPipeline;

bool debugMode = false;

#pragma endregion GameVariables

void Game::Init()
{
	// init sdl

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	// create window

	m_window = SDL_CreateWindow("Trainly 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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
	railModel = content.LoadModel("model/rail/Tory");

	unlitPipeline = {
		.vShader = content.LoadVertexShader("shader/unlit_vertex"),
		.pShader = content.LoadPixelShader("shader/unlit_pixel"),
	};

	litPipeline = {
		.vShader = content.LoadVertexShader("shader/lit_vertex"),
		.pShader = content.LoadPixelShader("shader/lit_pixel"),
	};

	float aspectRatio = 16.0 / 9.0f;
	cam = STRB::CreateRef<Camera>();
	cam->Init(30.0f, aspectRatio, 1.0f, 10000.0f);
	cam->SetPosition({ 1000.0f, 1000.0f, 0.0f});
	m_graphics->SetActiveCamera(cam);

	// load chunks

	auto chunk_paths = StringConverter::ReadAllLines("data/db/chunk.txt");

	for (auto& path : chunk_paths)
		m_chunks.push_back(content.LoadModel(path));

	Reload();

}

void Game::Reload()
{
	trainSpeed = 200;
	currentRail = 0;
	chunkQueue.clear();
	railQueue.clear();
	trainPos = Vector3::Zero;

	for (int i = 0; i < 300; i++)
	{
		int randomNumber = rand() % 2;
		chunkQueue.push_back(m_chunks[randomNumber]);
	}

	for (int i = 0; i < 100; i++)
	{
		railQueue.push_back(RailObject{
			.railModel = railModel,
			.pos = { 0.0f, 100.0f, railRootPos.z + (360.0f * i) },
			.rot = { 0.0f, 0.0f, float(rand() * 90), 0.0f }
		});
	}
}


void Game::Update(Timer& time)
{

	float cameraSpeed = 10.0f;

	if (Input.IsKeyPressed(SDLK_BACKQUOTE))
	{
		debugMode = !debugMode;
	}

	if (debugMode)
	{
		cam->SetFov(90.0f);

		if (Input.IsKeyDown(SDLK_e))
		{
			cam->AppendPosition(cam->GetUpVector() * cameraSpeed);
		}

		if (Input.IsKeyDown(SDLK_q))
		{
			cam->AppendPosition(cam->GetDownVector() * cameraSpeed);
		}

		if (Input.IsKeyDown(SDLK_w))
		{
			cam->AppendPosition(cam->GetForwardVector() * cameraSpeed);
		}

		if (Input.IsKeyDown(SDLK_s))
		{
			cam->AppendPosition(cam->GetBackwardVector() * cameraSpeed);
		}

		if (Input.IsKeyDown(SDLK_a))
		{
			cam->AppendPosition(cam->GetLeftVector() * cameraSpeed);
		}

		if (Input.IsKeyDown(SDLK_d))
		{
			cam->AppendPosition(cam->GetRightVector() * cameraSpeed);
		}

		if (Input.IsMouseButtonPressed(SDL_BUTTON_RIGHT))
			cam->AppendRotation({ (Input.GetDeltaMousePos().y * 0.005f), (Input.GetDeltaMousePos().x * 0.005f), 0.0f });
	}
	else
	{
		trainPos.z += time.DeltaTime() * trainSpeed;

		if (trainPos.z > railRootPos.z - 400.0f + (currentRail * 360.0f) && !godMode)
			Reload();

		cam->SetFov(35.0f);
		cam->SetPosition({ 1000.0f, 1000.0f, trainPos.z + 1500.0f });
		cam->SetRotation({ -100.0f, 180.0f, 0.0f });

		if (Input.IsKeyPressed(SDLK_SPACE))
		{
			if (int(railQueue[currentRail].rot.z) % 180 == 0)
			{
				railQueue[currentRail].pos.y = 0;
				currentRail++;
				trainSpeed += 10;
			}
			else if (!godMode)
			{
				Reload();
			}

		}

		if (Input.IsKeyPressed(SDLK_r))
		{
			railQueue[currentRail].rot.z += 90;
			std::cout << railQueue[currentRail].rot.z << std::endl;
		}
	}

	//cam->SetPosition(Vector3(100.0f, 1000.0f, 0.0f) + trainPos);
	//cam->SetLookTargetPosition(trainPos);

	
}


void Game::Draw(STRB::Ref<Graphics> renderer)
{
	renderer->Clear(37, 121, 231);

	
	updateFpsTimer += Time.DeltaTime();

	renderer->BindPipeline(unlitPipeline);

	for (int i = 0; i < railQueue.size(); i++)
	{
		renderer->DrawModel(*railQueue[i].railModel, railQueue[i].pos, railQueue[i].rot);
	}

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

	if (debugMode)
	{
		renderer->DrawString("FPS: " + std::to_string(fps), { 10.0f, 0.0f }, *debug);
		renderer->DrawString("Delta: " + std::to_string(deltaTime), { 10.0f, 35.0f }, *debug);

		// debug ui

		{
			ImGui::SetNextWindowSize({ 500, 50 });
			ImGui::Begin("Train Debug");
			ImGui::Text("Train position: X: %f, Y: %f, Z: %f", trainPos.x, trainPos.y, trainPos.z);
			ImGui::End();
		}

		{
			ImGui::Begin("Rail Debug");
			ImGui::Checkbox("god mode", &godMode);
			ImGui::Text("Current rail: %i", currentRail);
			if (ImGui::Button("Reset"))
				Reload();

			ImGui::End();
		}
	}
	else
	{
		renderer->DrawString("Score: " + std::to_string(currentRail), { 550, 80.0f }, *debug);
	}
	
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
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					m_graphics->Resize(e.window.data1, e.window.data2);
				}
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
	
		Draw(m_graphics);

		Update(Time);

		Input.Update();
	}


	Clean();
}


