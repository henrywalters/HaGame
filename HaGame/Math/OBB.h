#ifndef OBB_H
#define OBB_H

#include "../Utils/Aliases.h"
#include "./../Core/Object.h"

namespace hagame {
	namespace math {

		// Oriented bounding box defined by a center, radii and an orientation matrix. The orientation can easily be set from a quaternion
		struct OBB  {
			Vec3 center;
			Vec3 radius;
			Mat4 orientation;

			bool isIntersecting(OBB b) {
				float ra, rb;
				Mat3 R, AbsR;

				std::cout << orientation.toString() << std::endl;
				std::cout << b.orientation.toString() << std::endl;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) 
						R.set(i, j, dot(orientation.getRow(i).resize<3>(), b.orientation.getRow(j).resize<3>()));

				std::cout << R.toString() << std::endl;

				Vec3 t = b.center - center;

				t = Vec3({ 
					dot(t, orientation.getRow(0).resize<3>()),
					dot(t, orientation.getRow(1).resize<3>()),
					dot(t, orientation.getRow(2).resize<3>())
				});

				std::cout << t.toString() << std::endl;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						AbsR.set(i, j, std::abs(R.get(i, j)) + EPSILON);

				for (int i = 0; i < 3; i++) {
					ra = radius[i];
					rb = b.radius[0] * AbsR.get(i, 0) + b.radius[1] * AbsR.get(i, 1) + b.radius[2] * AbsR.get(i, 2);
					std::cout << ra << " " << rb << " " << t[i] << std::endl;
					if (std::abs(t[i]) > ra + rb) return false;
				}

				for (int i = 0; i < 3; i++) {
					ra = radius[0] * AbsR.getRow(0)[i] + radius[1] * AbsR.getRow(1)[i] + radius[2] * AbsR.getRow(2)[i];
					rb = b.radius[i];
					if (std::abs(t[0] * R.getRow(0)[i] + t[1] * R.getRow(1)[i] + t[2] * R.getRow(2)[i]) > ra + rb) return false;
				}

				ra = radius[1] * AbsR.get(2, 0) + radius[2] * AbsR.getRow(1)[0];
				rb = b.radius[1] * AbsR.getRow(0)[2] + b.radius[2] * AbsR.getRow(0)[1];
				if (std::abs(t[2] * R.getRow(1)[0] - t[1] * R.getRow(2)[0]) > ra + rb) return false;

				ra = radius[1] * AbsR.getRow(2)[1] + radius[2] * AbsR.getRow(1)[1];
				rb = b.radius[0] * AbsR.getRow(0)[2] + b.radius[2] * AbsR.getRow(0)[0];
				if (std::abs(t[2] * R.getRow(1)[1] - t[1] * R.getRow(2)[1]) > ra + rb) return false;

				ra = radius[1] * AbsR.getRow(2)[2] + radius[2] * AbsR.getRow(1)[2];
				rb = b.radius[0] * AbsR.getRow(0)[1] + b.radius[1] * AbsR.getRow(0)[0];
				if (std::abs(t[2] * R.getRow(1)[2] - t[1] * R.getRow(2)[2]) > ra + rb) return false;

				ra = radius[0] * AbsR.getRow(2)[0] + radius[2] * AbsR.getRow(0)[0];
				rb = b.radius[1] * AbsR.getRow(1)[2] + b.radius[2] * AbsR.getRow(1)[1];
				if (std::abs(t[0] * R.getRow(2)[2] - t[2] * R.getRow(0)[2]) > ra + rb) return false;

				ra = radius[0] * AbsR.getRow(2)[1] + radius[2] * AbsR.getRow(0)[1];
				rb = b.radius[0] * AbsR.getRow(1)[2] + b.radius[2] * AbsR.getRow(1)[0];
				if (std::abs(t[0] * R.getRow(2)[1] - t[2] * R.getRow(0)[1]) > ra + rb) return false;

				ra = radius[0] * AbsR.getRow(1)[0] + radius[1] * AbsR.getRow(0)[0];
				rb = b.radius[1] * AbsR.getRow(2)[2] + b.radius[2] * AbsR.getRow(2)[1];
				if (std::abs(t[1] * R.getRow(0)[0] - t[0] * R.getRow(1)[0]) > ra + rb) return false;

				ra = radius[0] * AbsR.getRow(1)[1] + radius[1] * AbsR.getRow(0)[1];
				rb = b.radius[0] * AbsR.getRow(2)[2] + b.radius[2] * AbsR.getRow(2)[0];
				if (std::abs(t[1] * R.getRow(0)[1] - t[0] * R.getRow(1)[1]) > ra + rb) return false;

				ra = radius[0] * AbsR.getRow(1)[2] + radius[1] * AbsR.getRow(0)[2];
				rb = b.radius[0] * AbsR.getRow(2)[1] + b.radius[1] * AbsR.getRow(2)[0];
				if (std::abs(t[1] * R.getRow(0)[2] - t[0] * R.getRow(1)[2]) > ra + rb) return false;

				return true;
			}

			String toString() {
				return "\ncenter: " + center.toString() + "\nradius: " + radius.toString() + "\norientation:\n" + orientation.toString();
			}
		};
	}
}

#endif
