#ifndef SURFACE_H
#define SURFACE_H

#include "./Triangle.h"
#include "./../Utils/Aliases.h"

namespace hagame {
	namespace math {
		struct Surface {
			Array<Triangle> triangles;

			Cube getBoundingCube() {
				bool init = false;
				Vec3 min, max;

				for (auto tri : triangles) {
					if (!init) {
						init = true;
						min = tri.a;
						max = tri.a;
					}

					for (int i = 0; i < 3; i++) {
						if (tri.a[i] < min[i])
							min[i] = tri.a[i];
						if (tri.a[i] > max[i])
							max[i] = tri.a[i];

						if (tri.b[i] < min[i])
							min[i] = tri.b[i];
						if (tri.b[i] > max[i])
							max[i] = tri.b[i];

						if (tri.c[i] < min[i])
							min[i] = tri.c[i];
						if (tri.c[i] > max[i])
							max[i] = tri.c[i];
					}
				}

				return Cube(min, max - min);
			}
		};
	}
}

#endif