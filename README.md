# HaGame
HaGame is a Game Engine developed in c++ for the purposes of developing my own games and experimenting with different techniques.

## Installation

Make HaGame installable as a lib

## Quick start

To get started with a custom game, you should define a class that extends the base game class like so:

```c++
#include "HaGame.h"

class DemoGame : public hagame::Game {
public:
	DemoGame(hagame::graphics::Window* window): Game(window) {}
}
```

Then in your main class, define a window and pass it to the game class:

```c++
#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "DemoGame.h"

int main() {
	hagame::Game::initializeSDL();

	// Window has many different overloads!
	auto window = hagame::graphics::Window(Vec2({ 800, 800 }), "Demo game");
	window.create();
	DemoGame game = DemoGame(&window);
	game.run();
}
```

## Architecture

## Notes

- You may notice duplicately named primitive classes in the graphics and math namespaces. Classes in the math namespace abstractly represent the underlying object whereas graphics classes represent a concrete renderable object and the means to render it