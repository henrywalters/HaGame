#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Goom2.h"
#include "SDL_mixer.h"

struct TestEvent {
	String message;

	TestEvent(String _message) : message(_message) {}
};

struct Data {
	Vec3 pos;
};

int main() {
	hagame::Game::initializeSDL();

	auto screenSize = hagame::graphics::MonitorManager::GetMonitor(0).displayBounds;

	// constexpr std::uint32_fast32_t HANDGUN_ID{ 1 << 0 };
	
	auto window = hagame::graphics::Window(screenSize.pos + Vec2({ 0, 30 }), screenSize.size - Vec2({ 0, 50 }), "Goom 2: back to hell, again");
	window.create();
	Goom2 goom = Goom2(&window);

	//Vec3 data = Vec3({ 1.0f, 2.0f, 3.0f });
	//nlohmann::json j = data;

	hagame::utils::FileSystem fs;
	//fs.getFile("test_data.json").write(j.dump());

	auto pos = JSON::parse(fs.getFile("test_data.json").read()).get<Vec3>();

	//goom.run();

	return 0;
}