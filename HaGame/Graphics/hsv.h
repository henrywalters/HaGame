#ifndef HSV_H
#define HSV_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		namespace color {
			struct HSV : public Vec3 {

				HSV() : HSV(0, 0, 0) {}
				HSV(float h, float s, float v) {
					vector[0] = h;
					vector[1] = s;
					vector[2] = v;
				}
			};
		}
	}
}

#endif
