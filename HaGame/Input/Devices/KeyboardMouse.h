#ifndef KEYBOARD_MOUSE
#define KEYBOARD_MOUSE

#include "../../Core/Publisher.h"
#include "../Device.h"
#include "../../Math/Vector.h"
#include "../../Math/Functions.h"
#include "../../Utils/Aliases.h"
#include "../../Vendor/imgui/imgui.h"
#include "../../Vendor/imgui/backends/imgui_impl_sdl.h"
#include "SDL.h"
#include <bitset>

namespace hagame {
	namespace input {
		namespace devices {

			using KeyCode = SDL_KeyCode;

			enum class MouseEvents {
				Moved,
				LeftPressed,
				LeftReleased,
				MiddlePressed,
				MiddleReleased,
				RightPressed,
				RightReleased
			};

			struct MouseEvent {

				MouseEvent(Vec2 pos) : mousePos(pos) {}

				Vec2 mousePos;
			};

			struct MouseState {
				int wheel;
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

			struct KeyboardState {
				bool numbers[10];
				bool numbersPressed[10];
				bool letters[26];
				bool lettersPressed[26];

				bool lCtrl, lCtrlPressed = false;
				bool rCtrl, rCtrlPressed = false;
				bool lShift, lShiftPressed = false;
				bool rShift, rShiftPressed = false;

				bool esc, escPressed = false;
				bool del, delPressed = false;
			};

			// Defines a mapping from a keyboard to generic device
			struct KeyboardMapping {
				KeyCode rUp, rRight, rDown, rLeft;
				KeyCode lUp, lRight, lDown, lLeft;
				KeyCode dUp, dRight, dDown, dLeft;

				KeyCode select, home, start;

				KeyCode a, b, x, y;

				KeyCode rTrigger, rShoulder, lTrigger, lShoulder;
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
						screenCenter = screenSize * 0.5;

						keyboardState = SDL_GetKeyboardState(NULL);
					}
				}

			public:

				Vec2 screenSize;
				Vec2 screenCenter;

				MouseState mouse;
				KeyboardState keyboard;

				Publisher<MouseEvents, MouseEvent> mouseEvents;

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
					std::cout << "SHOWING CURSOR\n";
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

					// ImGui_ImplSDL2_ProcessEvent(&event);

					switch (event.type) {
					case SDL_MOUSEMOTION:
						mouse.position[0] = event.motion.x;
						mouse.position[1] = event.motion.y;
						mouse.delta[0] = event.motion.xrel;
						mouse.delta[1] = event.motion.yrel;

						rAxis[0] = -mouse.delta[0];
						rAxis[1] = mouse.delta[1];

						mouseEvents.emit(MouseEvents::Moved, MouseEvent(mouse.position));

						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							updateBtnState(mouse.middle, mouse.middlePressed, true);
							mouseEvents.emit(MouseEvents::MiddlePressed, MouseEvent(mouse.position));
						}
						else if (event.button.button == SDL_BUTTON_LEFT) {
							updateBtnState(mouse.left, mouse.leftPressed, true);
							mouseEvents.emit(MouseEvents::LeftPressed, MouseEvent(mouse.position));
						}
						else if (event.button.button == SDL_BUTTON_RIGHT) {
							updateBtnState(mouse.right, mouse.rightPressed, true);
							mouseEvents.emit(MouseEvents::RightPressed, MouseEvent(mouse.position));
						}
						break;
					case SDL_MOUSEBUTTONUP:
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							updateBtnState(mouse.middle, mouse.middlePressed, false);
							mouseEvents.emit(MouseEvents::MiddleReleased, MouseEvent(mouse.position));
						}
						else if (event.button.button == SDL_BUTTON_LEFT) {
							updateBtnState(mouse.left, mouse.leftPressed, false);
							mouseEvents.emit(MouseEvents::LeftReleased, MouseEvent(mouse.position));
						}
						else if (event.button.button == SDL_BUTTON_RIGHT) {
							updateBtnState(mouse.right, mouse.rightPressed, false);
							mouseEvents.emit(MouseEvents::RightReleased, MouseEvent(mouse.position));
						}
						break;
					case SDL_MOUSEWHEEL:
						mouse.wheel = event.wheel.y;
						break;
					}
				}

				void reset() {
					mouse.delta = Vec2::Zero();

					mouse.wheel = 0;
					mouse.leftPressed = false;
					mouse.rightPressed = false;
					mouse.middlePressed = false;

					rAxis = Vec2::Zero();
					lAxis = Vec2::Zero();
					dPad = Vec2::Zero();
				}

				void pollDevice() {

					init();

					SDL_PumpEvents();

					lAxis[0] = handleAxis(isKeyDown(SDLK_d), isKeyDown(SDLK_a));
					lAxis[1] = handleAxis(isKeyDown(SDLK_w), isKeyDown(SDLK_s));

					dPad[0] = handleAxis(isKeyDown(SDLK_RIGHT), isKeyDown(SDLK_LEFT));
					dPad[1] = handleAxis(isKeyDown(SDLK_UP), isKeyDown(SDLK_DOWN));

					updateBtnState(select, selectPressed, isKeyDown(SDLK_TAB));
					updateBtnState(start, startPressed, isKeyDown(SDLK_ESCAPE));
					updateBtnState(a, aPressed, isKeyDown(SDLK_SPACE));
					updateBtnState(b, bPressed, isKeyDown(SDLK_LSHIFT));

					updateBtnState(keyboard.lCtrl, keyboard.lCtrlPressed, isKeyDown(SDLK_LCTRL));
					updateBtnState(keyboard.rCtrl, keyboard.rCtrlPressed, isKeyDown(SDLK_RCTRL));
					updateBtnState(keyboard.esc, keyboard.escPressed, isKeyDown(SDLK_ESCAPE));
					updateBtnState(keyboard.del, keyboard.delPressed, isKeyDown(SDLK_DELETE));
					updateBtnState(keyboard.lShift, keyboard.lShiftPressed, isKeyDown(SDLK_LSHIFT));
					updateBtnState(keyboard.rShift, keyboard.rShiftPressed, isKeyDown(SDLK_RSHIFT));
					updateBtnState(lTrigger, lTriggerPressed, mouse.right);
					updateBtnState(rTrigger, rTriggerPressed, mouse.left);

					for (int i = 0; i < 10; i++) {
						updateBtnState(keyboard.numbers[i], keyboard.numbersPressed[i], isKeyDown(SDLK_0 + i));
					}

					for (int i = 0; i < 26; i++) {
						updateBtnState(keyboard.letters[i], keyboard.lettersPressed[i], isKeyDown(SDLK_a + i));
					}
				}

				void updateImgui(double dt) {
					ImGuiIO& io = ImGui::GetIO();
					io.DeltaTime = dt;
					io.MousePos = ImVec2(mouse.position[0], mouse.position[1]);
					io.MouseDown[0] = mouse.left;
					io.MouseDown[1] = mouse.right;
					io.MouseWheel = static_cast<float>(mouse.wheel);
					io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
				}
			};
		}
	}
}

#endif