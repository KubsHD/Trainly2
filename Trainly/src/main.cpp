#include "stdafx.h"

#include "graphics/graphics.h"
#include "timer.h"
#include "Input.h"
#include "io/ResourceManager.h"

using namespace DirectX;

SDL_Window* m_window;
Graphics* graphics;

STRB::Ref<Model> test;
STRB::Ref<Font> debug;

Input input;

Timer t;

ResourceManager rm;

void update()
{

}

Pipeline unlitPipeline;

void init()
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

    graphics = new Graphics();


    graphics->Init(wmInfo.info.win.window, m_window);


    rm.Init("data", *graphics);

	graphics->CreateTriangle();


    //SDL_WarpMouseInWindow(m_window, 640, 360); 
    //SDL_SetWindowGrab(m_window, SDL_TRUE);


    //SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_CaptureMouse(SDL_TRUE);

    test = rm.LoadModel("model/chunk/farm/mdl_farm_chunk1");
    debug = rm.LoadFont("font/comic");

    unlitPipeline = {
		.vShader = rm.LoadVertexShader("shader/unlit_vertex"),
        .pShader = rm.LoadPixelShader("shader/unlit_pixel"),
    };

}

float counter = 0;

float updateFpsTimer = 0;

float fps = 0;
float deltaTime = 0;


void frame()
{
    graphics->Clear(37, 121, 231);

    counter += t.DeltaTime();
    updateFpsTimer += t.DeltaTime();

    graphics->DrawTraingle(counter, input);

    graphics->BindPipeline(unlitPipeline);

    graphics->DrawModel(*test, {0.0f, 0.0f, 7200.0f});


    if (updateFpsTimer >= 0.3)
    {
        fps = 1.0f / t.DeltaTime();
        deltaTime = t.DeltaTime();
        updateFpsTimer = 0;
    }

    graphics->GetSpriteBatch()->Begin();
    graphics->DrawString("FPS: " + std::to_string(fps), { 10.0f, 0.0f }, *debug);
	graphics->DrawString("Delta: " + std::to_string(deltaTime), { 10.0f, 35.0f }, *debug);
	graphics->GetSpriteBatch()->End();


    graphics->Present();

}

void clean()
{
    delete graphics;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void run()
{
    init();

    bool running = true;

    while (running)
    {
        SDL_PumpEvents();

        t.Tick();

        frame();

        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                input.SetKeyState(e.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                input.SetKeyState(e.key.keysym.sym, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                input.SetMouseButtonState(e.button.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                input.SetMouseButtonState(e.button.button, false);
                break;
            default:
                break;
            }
        }

        int x, y;
        SDL_GetRelativeMouseState(&x, &y);
        input.SetDeltaMouseState({ (float)x , (float)y });

        SDL_GetMouseState(&x, &y);
        input.SetMousePosition({ (float)x , (float)y });
    }


    clean();
}

int main(int argc, char* argv[])
{
    run();

    return 0;

}
