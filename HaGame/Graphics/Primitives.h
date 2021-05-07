#ifndef PRIMITIVES
#define PRIMITIVES 

#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include "Mesh.h"
#include <math.h>

namespace hagame {
	namespace graphics {

		const float SR_3 = sqrt(3) / 2.0f;

		const Array<float> QuadVertices = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
		};

	}
}

#endif