#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL.h"
#include "Devices/KeyboardMouse.h"
#include "Devices/Gamepad.h"
#include "../Utils/Aliases.h"
#include "../Core/Publisher.h"

namespace hagame {
	namespace input {

		enum WindowEvents {
			Moved,
			Resized,
		};

		struct WindowEvent {
			uint32_t windowId;
			Vec2 data;
		};

		class InputManager {

			// Flick this depending on the last input if it was a keyboard/mouse or controller event for player 1.
			bool playerOneUseController = false;

			std::function<void()> _onQuit = []() {};

			void handleWindowEvent(SDL_WindowEvent event) {
				switch (event.event) {
				case SDL_WINDOWEVENT_MOVED:
					windowEvents.emit(WindowEvents::Moved, WindowEvent{ event.windowID, Vec2({(float)event.data1, (float)event.data2}) });
					break;
				case SDL_WINDOWEVENT_RESIZED:
					windowEvents.emit(WindowEvents::Resized, WindowEvent{ event.windowID, Vec2({(float)event.data1, (float)event.data2}) });
					break;
				}
			}

			void handleControllerEvent(SDL_ControllerDeviceEvent event) {
				if (event.type == SDL_CONTROLLERDEVICEADDED) {
					std::cout << "\nConnected joy device: " << event.which << "\n";
					gamepads.insert(std::make_pair(event.which, devices::Gamepad(event.which)));
				}

				if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
					std::cout << "\nDisconnected joy device: " << event.which << "\n";
					gamepads.erase(event.which);
				}
			}

			bool isControllerEvent(SDL_Event event) {
				return event.type == SDL_CONTROLLERDEVICEADDED || event.type == SDL_CONTROLLERDEVICEREMOVED || event.type == SDL_CONTROLLERDEVICEREMAPPED;
			}

			bool isMouseEvent(SDL_Event event) {
				return event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEWHEEL;
			}

			bool isKeyboardEvent(SDL_Event event) {
				return event.type == SDL_KEYUP || event.type == SDL_KEYDOWN;
			}

			bool isControllerInputEvent(SDL_Event event) {
				return event.type == SDL_CONTROLLERAXISMOTION || event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP;
			}

		public:

			Publisher<WindowEvents, WindowEvent> windowEvents;
			devices::KeyboardMouse keyboardMouse;
			Map<int, devices::Gamepad> gamepads;

			void onQuit(std::function<void()> lambda) {
				_onQuit = lambda;
			}

			void pollEvents() {
				SDL_Event event;

				keyboardMouse.reset();

				for (auto &[idx, gamepad] : gamepads) {
					gamepad.reset();
				}

				while (SDL_PollEvent(&event)) {
					
					if (event.type == SDL_QUIT) {
						_onQuit();
					}

					if (event.type == SDL_WINDOWEVENT) {
						handleWindowEvent(event.window);
					}

					if (isControllerEvent(event))
						handleControllerEvent(event.cdevice);

					if (isKeyboardEvent(event) || isMouseEvent(event)) {
						playerOneUseController = false;
						keyboardMouse.handleEvent(event);
					}

					if (isControllerInputEvent(event)) {
						for (auto& [idx, gamepad] : gamepads) {
							if (idx == 0)
								playerOneUseController = true;

							gamepad.handleEvent(event);
						}
					}
				}

				keyboardMouse.pollDevice();

				for (auto& [idx, gamepad] : gamepads) {
					gamepad.pollDevice();
				}
			}

			bool playerExists(int index) {
				return index == 0 || gamepads.find(index) != gamepads.end();
			}

			bool usingGamepad(int index) {
				if (!playerExists(index)) {
					throw new std::exception("Attempting to get player that does not exist.");
				}

				return index == 0 && playerOneUseController;
			}

			String playerToString(int index) {
				return usingGamepad(index) ? "Gamepad" : "Keyboard and mouse";
			}

			DeviceState player(int index) {
				if (!playerExists(index)) {
					throw new std::exception("Attempting to get player that does not exist.");
				}

				if (usingGamepad(index))
					return gamepads[index];
				else
					return keyboardMouse;
			}
		};
	}
}

#endif
