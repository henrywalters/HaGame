// HaGame3D_v1.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED // insert this
#include "HaGame3D_v1.h"
#include "SDL.h"

using Vec3 = hagame::math::Vector<3, double>;

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    while (true) {
        SDL_SetRenderDrawColor(renderer, 155, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}
