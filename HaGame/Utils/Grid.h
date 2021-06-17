#ifndef GRID_H
#define GRID_H

#include "./Vector.h"

namespace hagame {
	namespace math {

		// Provides a storage container for data partioned into a grid
		template <size_t Rows, size_t Cols, class DataType>
		class Grid {

			DataType grid[Rows][Cols];

		public:
			Grid() {}
			
		};
	}
}

#endif
