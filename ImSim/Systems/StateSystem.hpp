#ifndef STATE_SYSTEM_HPP
#define STATE_SYSTEM_HPP

#include "../../HaGame/HaGame.h"

struct State {
	bool debug = false;
	bool lockMouse = false;
	bool vsync = false;
};

enum StateEvents {
	DebugChange,
	LockMouseChange,
	VSyncChange,
};

class StateSystem : public hagame::ecs::System {


public:

	Ptr<State> state;
	Ptr<hagame::Publisher<StateEvents, Ptr<State>>> events;

	StateSystem() {
		events = std::make_shared<hagame::Publisher<StateEvents, Ptr<State>>>();
		state = std::make_shared<State>();
	}

	String getSystemName() {
		return "StateSystem";
	}

	void onSystemUpdate(double dt) {
		if (game->input.keyboardMouse.keyboard.escPressed) {
			game->running = false;
		}

		if (game->input.keyboardMouse.keyboard.lettersPressed[getCharIndex('p')]) {
			state->debug = !state->debug;
			events->emit(StateEvents::DebugChange, state);
		}

		if (game->input.keyboardMouse.keyboard.lettersPressed[getCharIndex('l')]) {
			state->lockMouse = !state->lockMouse;
			events->emit(StateEvents::LockMouseChange, state);
		}

		if (game->input.keyboardMouse.keyboard.lettersPressed[getCharIndex('v')]) {
			state->vsync = !state->vsync;
			events->emit(StateEvents::VSyncChange, state);
		}
	}
};

#endif
