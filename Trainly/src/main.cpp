#include "stdafx.h"

#include "graphics/graphics.h"
#include "timer.h"
#include "Input.h"

SDL_Window* m_window;
Graphics* graphics;

Input input;

Timer t;

void update()
{

}

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

    graphics->CreateTriangle();

    //SDL_WarpMouseInWindow(m_window, 640, 360); 
    //SDL_SetWindowGrab(m_window, SDL_TRUE);


    //SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_CaptureMouse(SDL_TRUE);

}

float counter = 0;

void frame()
{
    graphics->Clear(37, 121, 231);

    counter += t.DeltaTime();

    // graphics->DrawString(std::string(t.DeltaTime()), "comic"));

    graphics->DrawTraingle(counter, input);

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
