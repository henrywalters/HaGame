#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL.h"
#include "Devices/KeyboardMouse.h"
#include "Devices/Gamepad.h"
#include "../Utils/Aliases.h"
#include "../Core/Publisher.h"

namespace hagame {
	namespace input {
		class InputManager {
			std::function<void()> _onQuit = []() {};
		public:

			devices::KeyboardMouse keyboardMouse;
			Array<devices::Gamepad> gamepads;

			void onQuit(std::function<void()> lambda) {
				_onQuit = lambda;
			}

			void pollEvents() {
				SDL_Event event;

				keyboardMouse.pollDevice();

				for (auto gamepad : gamepads) {
					gamepad.pollDevice();
				}

				while (SDL_PollEvent(&event)) {
					
					if (event.type == SDL_QUIT) {
						_onQuit();
					}

					keyboardMouse.handleEvent(event);
					for (auto gamepad : gamepads) {
						gamepad.handleEvent(event);
					}
				}
			}
		};
	}
}

#endif
