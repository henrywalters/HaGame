#ifndef KEYBOARD_MOUSE
#define KEYBOARD_MOUSE

#include "../Device.h"
#include "../../Math/Vector.h"
#include "../../Math/Functions.h"
#include "../../Utils/Aliases.h"
#include "SDL.h"

namespace hagame {
	namespace input {
		namespace devices {

			const int MAX_MOUSE_MOVE = 1000;
			class KeyboardMouse : public hagame::input::Device {
			private:

				bool initialized;
				const Uint8* keyboardState;

				void checkKey(bool& btn, bool& btnPressed, SDL_Keycode key) {
					updateBtnState(btn, btnPressed, isKeyDown(key));
				}

				int handleAxis(bool key1, bool key2) {
					if (key1 && !key2) return 1;
					else if (key2 && !key1) return -1;
					else return 0;
				}

				void handleMouse() {
					prevMousePos = mousePos;
					mousePos = getMousePos();
					mouseDelta = prevMousePos - mousePos;
				}

				void init() {
					if (!initialized) {
						initialized = true;
						prevMousePos = getMousePos();
						mousePos = prevMousePos;
						
						SDL_DisplayMode dm;
						SDL_GetCurrentDisplayMode(0, &dm);
						screenSize[0] = dm.w;
						screenSize[1] = dm.h;
						std::cout << "Screen Size: " << screenSize.toString() << "\n";
						screenCenter = screenSize * 0.5;
					}
				}

			public:

				Vec2 screenSize;
				Vec2 screenCenter;
				Vec2 mousePos;
				Vec2 prevMousePos;
				Vec2 mouseDelta;

				KeyboardMouse() {
					initialized = false;
					prevMousePos = Vec2::Zero();
				}

				bool isKeyDown(SDL_Keycode key) {
					return keyboardState[SDL_GetScancodeFromKey(key)];
				}

				Vec2 getMousePos() {
					int x, y;
					SDL_GetMouseState(&x, &y);
					return Vec2({ (float) x , (float) y });
				}

				void pollDevice() {
					init();
					handleMouse();
					keyboardState = SDL_GetKeyboardState(NULL);
					rAxis = Vec2::Zero();
					lAxis = Vec2::Zero();
					dPad = Vec2::Zero();

					lAxis[0] = handleAxis(isKeyDown(SDLK_d), isKeyDown(SDLK_a));
					lAxis[1] = handleAxis(isKeyDown(SDLK_s), isKeyDown(SDLK_w));

					rAxis[0] = normalize<float>(mousePos[0], MAX_MOUSE_MOVE);
					rAxis[1] = normalize<float>(mousePos[1], MAX_MOUSE_MOVE);

					checkKey(home, homePressed, SDLK_ESCAPE);
				}
			};
		}
	}
}

#endif