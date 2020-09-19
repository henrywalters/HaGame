// HaGame3D_v1.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED // insert this
#include "HaGame3D_v1.h"
#include "SDL.h"
#include "GL/glew.h"
#include "SDL_opengl.h"

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
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext context;

    glewInit();

    context = SDL_GL_CreateContext(window);

    if (context == NULL) {
        std::cout << SDL_GetError() << std::endl;
    }

    while (true) {

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}
