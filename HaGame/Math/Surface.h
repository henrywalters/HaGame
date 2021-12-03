#ifndef SURFACE_H
#define SURFACE_H

#include "./Triangle.h"

namespace hagame {
	namespace math {
		struct Surface {
			Array<Triangle> triangles;
		};
	}
}

#endif