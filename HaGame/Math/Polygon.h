#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <cmath>
#include "Vector.h"

namespace hagame {
	namespace math {

		template <size_t Dim, class T>
		class Polygon {
		public:
			std::vector<Vector<Dim, T>> points;
		};
	}
}

#endif
