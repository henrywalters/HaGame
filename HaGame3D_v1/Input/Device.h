#include "../Math/Vector.h"

#ifndef DEVICE
#define DEVICE

namespace hagame {
	namespace input {
		struct DeviceState {
			hagame::math::Vector<2, double> rAxis;
			hagame::math::Vector<2, double> lAxis;
			hagame::math::Vector<2, double> dPad;

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
		public:
			virtual void pollDevice() {
				throw new std::exception("pollDevice unimplemented for hagame::input::Device");
			};
		};
	}
}

#endif