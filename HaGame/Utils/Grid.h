#ifndef UTILS_GRID_H
#define UTILS_GRID_H

#include <unordered_map>

namespace hagame {
	namespace utils {
		// Convience class for a simple grid container
		template <class T>
		class Grid {
		public:

			bool has(int col, int row) {
				return m_grid.find(col) != m_grid.end() && m_grid[col].find(row) != m_grid[col].end();
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

		private:
			std::unordered_map<int, std::unordered_map<int, T>> m_grid;
			size_t m_size;
		};
	}
}

#endif