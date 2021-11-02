#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Goom2.h"
#include "SDL_mixer.h"

struct TestEvent {
	String message;

	TestEvent(String _message) : message(_message) {}
};

struct Data {
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, pos);
	Vec3 pos;
};

int main() {
	hagame::Game::initializeSDL();

	auto screenSize = hagame::graphics::MonitorManager::GetMonitor(0).displayBounds;

	// constexpr std::uint32_fast32_t HANDGUN_ID{ 1 << 0 };
	
	auto window = hagame::graphics::Window(screenSize.pos + Vec2({ 0, 30 }), screenSize.size - Vec2({ 0, 50 }), "Goom 2: back to hell, again");
	window.create();
	Goom2 goom = Goom2(&window);

	hagame::utils::UUID id1, id2, id3, id4;

	std::cout << id1 << "\n" << id2 << "\n" << id4 << "\n";

	goom.run();

	return 0;
}