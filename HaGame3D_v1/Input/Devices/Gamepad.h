#include "../Device.h"
#include "../../Math/Vector.h"
#include "SDL.h"

namespace hagame {
	namespace input {
		namespace devices {

			const int JOYSTICK_DEADZONE = 3000;

			class Gamepad: public hagame::input::Device {
			private:
				SDL_GameController* joystick;
				SDL_Haptic* haptic;
				bool active = false;

				void checkButton(bool& btn, bool& btnPressed, SDL_GameControllerButton button) {
					checkButton(btn, btnPressed, SDL_GameControllerGetButton(joystick, button));
				}

				void checkButton(bool& btn, bool& btnPressed, bool state) {
					btnPressed = false;
					if (state) {
						if (!btn) btnPressed = true;
						btn = true;
					}
					else {
						btn = false;
					}
				}
			public:

				Gamepad(int index) {
					joystick = SDL_GameControllerOpen(index);

					haptic = SDL_HapticOpen(index);
					SDL_HapticRumbleInit(haptic);
					if (joystick == NULL) {
						std::cout << "Warning: failed to open joystick " << index << " - Error: " << SDL_GetError() << std::endl;
					}
					else {
						active = true;
					}
				}

				void rumble(float strength, float dur) {
					if (active) {
						if (SDL_HapticRumblePlay(haptic, strength, dur) != 0) {
							std::cout << "Failed to rumble: " << SDL_GetError() << std::endl;
						}
					}
				}

				void pollDevice() {
					rAxis = hagame::math::Vector<2, double>::Zero();
					lAxis = hagame::math::Vector<2, double>::Zero();
					dPad = hagame::math::Vector<2, double>::Zero();

					if (active) {
						lAxis[0] = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTX);
						lAxis[1] = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTY);
						rAxis[0] = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTX);
						rAxis[1] = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTY);

						lAxis[0] = abs(lAxis[0]) > JOYSTICK_DEADZONE ? lAxis[0] / 32768.0 : 0;
						lAxis[1] = abs(lAxis[1]) > JOYSTICK_DEADZONE ? lAxis[1] / 32768.0 : 0;
						rAxis[0] = abs(rAxis[0]) > JOYSTICK_DEADZONE ? rAxis[0] / 32768.0 : 0;
						rAxis[1] = abs(rAxis[1]) > JOYSTICK_DEADZONE ? rAxis[1] / 32768.0 : 0;

						checkButton(a, aPressed, SDL_CONTROLLER_BUTTON_A);
						checkButton(b, bPressed, SDL_CONTROLLER_BUTTON_B);
						checkButton(x, xPressed, SDL_CONTROLLER_BUTTON_X);
						checkButton(y, yPressed, SDL_CONTROLLER_BUTTON_Y);

						checkButton(select, selectPressed, SDL_CONTROLLER_BUTTON_BACK);
						checkButton(home, homePressed, SDL_CONTROLLER_BUTTON_GUIDE);
						checkButton(start, startPressed, SDL_CONTROLLER_BUTTON_START);

						checkButton(rShoulder, rShoulderPressed, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
						checkButton(lShoulder, lShoulderPressed, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

						lTriggerRaw = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
						rTriggerRaw = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

						checkButton(lTrigger, lTriggerPressed, lTriggerRaw >= JOYSTICK_DEADZONE);
						checkButton(rTrigger, rTriggerPressed, rTriggerRaw >= JOYSTICK_DEADZONE);
					}
				}
			};
		}
	}
}