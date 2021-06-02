#ifndef OBB_H
#define OBB_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace math {

		// Oriented bounding box defined by a center, radii and an orientation matrix. The orientation can easily be set from a quaternion
		struct OBB {
			Vec3 center;
			Vec3 radius;
			Mat4 orientation;

			bool isIntersecting(OBB b) {
				float ra, rb;
				Mat3 R, AbsR;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) 
						R[i][j] = dot(orientation.getCol(i), b.orientation.getCol(j));

				Vec3 t = b.center - a.center;
				t = Vec3({ dot(t, orientation.getCol(0), dot(t, orientation.getCol(1)), dot(t, orientation.getCo(2)) });

				R.forEach([&AbsR](int row, int col) {
					AbsR.set(row, col, abs(R.get(row, col))) + EPSILON;
				});

				for (int i = 0; i < 3; i++) {
					// ra = 
				}
			}
		};
	}
}

#endif
