#ifndef GRID_MAP_H
#define GRID_MAP_H

#include "./Aliases.h"

namespace hagame {
	namespace utils {

		// Provides a Map data structure of a grid index to an object
		template <class DataType>
		class GridMap {

			Map<int, Map<int, DataType>> grid;

			bool hasRow(int row) {
				return grid.find(row) != grid.end();
			}

			bool hasCell(int row, int col) {
				return hasRow(row) && grid[row].find(col) != grid[row].end();
			}

		public:
			GridMap() {}
			~GridMap() {
				clear();
			}

			void set(int row, int col, DataType value) {
				if (!hasRow(row)) {
					grid.insert(std::make_pair(row, Map<int, DataType>()));
				}

				grid[row].insert(std::make_pair(col, value));
			}

			Optional<DataType> get(int row, int col) {
				return hasCell(row, col) ? Optional<grid[row][col]> : std::nullopt;
			}

			DataType getOrFail(int row, int col) {
				if (!hasCell(row, col))
					throw new std::exception("Cell does not exist in grid map");
				return grid[row][col];
			}

			void clear() {
				grid.clear();
			}
		};
	}
}

#endif
