#ifndef UTILS_GRID_H
#define UTILS_GRID_H

#include <unordered_map>

namespace hagame {
	namespace utils {
		// Convience class for a simple grid container
		template <class T>
		class Grid {
		public:

			struct Index {
				int col, row;
				String toString() { return std::to_string(col) + "," + std::to_string(row); }
			};

			struct Range {
				Index start, end;

				String toString() { start.toString() + ":" + end.toString(); }
			};

			bool has(int col, int row) {
				return m_grid.find(col) != m_grid.end() && m_grid[col].find(row) != m_grid[col].end();
			}

			bool has(Index index) {
				return has(index.col, index.row);
			}

			bool has(Vec2Int index) {
				return has(index[0], index[1]);
			}

			void insert(int col, int row, T value) {
				if (m_grid.find(col) == m_grid.end()) {
					m_grid.insert(std::make_pair(col, std::unordered_map<int, T>()));
				}
				m_grid[col][row] = value;
				m_size++;
			}

			T get(int col, int row) {
				return m_grid[col][row];
			}

			T get(Index index) {
				return m_grid[index.col][index.row];
			}

			T get(Vec2Int index) {
				return m_grid[index[0]][index[1]];
			}

			T* getRef(Vec2Int index) {
				return &m_grid[index[0]][index[1]];
			}

			void remove(int col, int row) {
				if (m_grid.find(col) != m_grid.end()) {
					if (m_grid[col].find(row) != m_grid[col].end()) {
						m_grid[col].erase(row);
						m_size--;
					}
					
				}
			}

			void clear() {
				m_grid.clear();
				m_size = 0;
			}

			size_t size() {
				return m_size;
			}

			void forEach(std::function<void(T)> lambda) {
				for (auto &[colIdx, row] : m_grid) {
					for (auto& [rowIdx, cell] : row) {
						lambda(cell);
					}
				}
			}

			void forEach(std::function<void(int, int, T)> lambda) {
				for (auto& [colIdx, row] : m_grid) {
					for (auto& [rowIdx, cell] : row) {
						lambda(colIdx, rowIdx, cell);
					}
				}
			}

			void forEach(std::function<void(int, int, T*)> lambda) {
				for (auto& [colIdx, row] : m_grid) {
					for (auto& [rowIdx, cell] : row) {
						lambda(colIdx, rowIdx, &cell);
					}
				}
			}

			Range getRange() {
				Range range;

				bool init = false;

				forEach([&range, &init](int col, int row, T el) {
					if (!init) {
						range.start.col = col;
						range.start.row = row;
						range.end.col = col;
						range.end.row = row;
						init = true;
					}
					else {
						if (col < range.start.col) range.start.col = col;
						if (row < range.start.row) range.start.row = row;
						if (col > range.end.col) range.end.col = col;
						if (row > range.end.row) range.end.row = row;
					}
				});

				return range;
			}

		private:
			std::unordered_map<int, std::unordered_map<int, T>> m_grid;
			size_t m_size;
		};
	}
}

#endif