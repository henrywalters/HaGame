#ifndef DEVICE
#define DEVICE

#include "../Math/Vector.h"

namespace hagame {
	namespace input {
		struct DeviceState {
			Vec2 rAxis;
			Vec2 lAxis;
			Vec2 dPad;

			bool select, selectPressed;
			bool home, homePressed;
			bool start, startPressed;

			bool a, aPressed;
			bool b, bPressed;
			bool x, xPressed;
			bool y, yPressed;

			float rTriggerRaw, lTriggerRaw;
			bool lTrigger, lTriggerPressed;
			bool rTrigger, rTriggerPressed;

			bool lShoulder, lShoulderPressed;
			bool rShoulder, rShoulderPressed;
		};

		class Device : public DeviceState {
		protected:
			virtual void updateBtnState(bool& btn, bool& btnPressed, bool state) {
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
			virtual void pollDevice() {
				throw new Exception("pollDevice unimplemented");
			};
		};
	}
}

#endif