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

			struct MouseState {
				Vec2 position;
				Vec2 prevPosition;
				Vec2 delta;

				bool left = false;
				bool leftPressed = false;
				bool middle = false;
				bool middlePressed = false;
				bool right = false;
				bool rightPressed = false;

				Vec2 getRelativePosition(Rect viewport) {
					return position - viewport.pos;
				}
			};

			const int MAX_MOUSE_MOVE = 500;
			class KeyboardMouse : public hagame::input::Device {
			private:

				bool relative = false;
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
					mouse.position = getMousePos();
				}

				void init() {
					if (!initialized) {
						initialized = true;
						mouse.prevPosition = getMousePos();
						mouse.position = mouse.prevPosition;
						
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

				MouseState mouse;

				KeyboardMouse() {
					initialized = false;
					mouse.prevPosition = Vec2::Zero();
				}

				bool isKeyDown(SDL_Keycode key) {
					return keyboardState[SDL_GetScancodeFromKey(key)];
				}

				Vec2 getMousePos() {
					int x, y;
					if (relative) {
						SDL_GetRelativeMouseState(&x, &y);
					}
					else {
						SDL_GetMouseState(&x, &y);
					}
					return Vec2({ (float) x , (float) y });
				}

				void showCursor() {
					SDL_ShowCursor(SDL_ENABLE);
				}

				void hideCursor() {
					SDL_ShowCursor(SDL_DISABLE);
				}

				void captureMouseOn() {
					SDL_SetRelativeMouseMode(SDL_TRUE);
					relative = true;
				}

				void captureMouseOff() {
					SDL_SetRelativeMouseMode(SDL_FALSE);
					relative = false;
				}

				void toggleMouseCapture() {
					if (relative) {
						captureMouseOff();
					}
					else {
						captureMouseOn();
					}
				}

				void handleEvent(SDL_Event event) {

					switch (event.type) {
					case SDL_MOUSEMOTION:
						mouse.position[0] = event.motion.x;
						mouse.position[1] = event.motion.y;
						mouse.delta[0] = event.motion.xrel;
						mouse.delta[1] = event.motion.yrel;

						rAxis[0] = -mouse.delta[0] / 10.0f;
						rAxis[1] = mouse.delta[1] / 10.0f;
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							updateBtnState(mouse.middle, mouse.middlePressed, true);
						}
						else if (event.button.button == SDL_BUTTON_LEFT) {
							updateBtnState(mouse.left, mouse.leftPressed, true);
						}
						else if (event.button.button == SDL_BUTTON_RIGHT) {
							updateBtnState(mouse.right, mouse.rightPressed, true);
						}
						break;
					case SDL_MOUSEBUTTONUP:
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							updateBtnState(mouse.middle, mouse.middlePressed, false);
						}
						else if (event.button.button == SDL_BUTTON_LEFT) {
							updateBtnState(mouse.left, mouse.leftPressed, false);
						}
						else if (event.button.button == SDL_BUTTON_RIGHT) {
							updateBtnState(mouse.right, mouse.rightPressed, false);
						}
						break;

					}
				}

				void pollDevice() {
					init();
					mouse.delta = Vec2::Zero();
					//handleMouse();

					keyboardState = SDL_GetKeyboardState(NULL);
					rAxis = Vec2::Zero();
					lAxis = Vec2::Zero();
					dPad = Vec2::Zero();

					lAxis[0] = handleAxis(isKeyDown(SDLK_a), isKeyDown(SDLK_d));
					lAxis[1] = handleAxis(isKeyDown(SDLK_w), isKeyDown(SDLK_s));
					

					updateBtnState(select, selectPressed, isKeyDown(SDLK_TAB));
					updateBtnState(start, startPressed, isKeyDown(SDLK_ESCAPE));
					updateBtnState(a, aPressed, isKeyDown(SDLK_SPACE));
				}
			};
		}
	}
}

#endif