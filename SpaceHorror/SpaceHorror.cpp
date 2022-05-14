#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./SpaceHorror.h"
#include "./../HaGame/Utils/Promise.h"
#include "./../HaGame/Utils/TimedCallback.h"

#include "./../HaGame/Utils/DataStructures/CyclicalLinkedList.h"

int main() {

	hagame::Game::initializeSDL();

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(0); 
	auto window = hagame::graphics::Window(
		Vec2(1400, 900),
		"SpaceHorror"
	);


	auto cl = hagame::utils::CyclicalLinkedList<int>();

	for (int i = 0; i < 10; i++) {
		cl.push(i);
	}

	for (int i = 5; i >= 0; i--) {
		cl.pushBefore(i);
	}

	while (!cl.isEmpty()) {
		std::cout << cl.value() << "\n";
		cl.pop();
	}

	window.create();

	SpaceHorror fuckMeUp(&window);

	fuckMeUp.run();


	return 0;
}