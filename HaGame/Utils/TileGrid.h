#ifndef TILE_GRID_H
#define TILE_GRID_H

#include "./Grid.h"

namespace hagame {
	namespace utils {
		
		// TileGrid provides a Grid structure with a fixed cell size
		template <class T>
		class TileGrid : public Grid<T> {
		public:

			struct Edge {
				Vec2 start;
				Vec2 end;
			};

			struct Cell {
				int edgeId[4];
				bool edgeExists[4];
			};

			TileGrid(Vec2 cellSize) :
				m_cellSize(cellSize) {}

			// Compute the individual edges formed by the tiles
			Array<Edge> getEdges() {

				const int NORTH = 0;
				const int EAST = 1;
				const int SOUTH = 2;
				const int WEST = 3;

				Array<Edge> edges;
				Grid<Cell> cells;

				forEach([&cells](int col, int row, T el) {
					Cell cell;
					for (int i = 0; i < 4; i++) {
						cell.edgeId[i] = 0;
						cell.edgeExists[i] = false;
					}
					cells.insert(col, row, cell);
				});

				Grid<T>::Range range = getRange();

				// Use the range instead of forEach to control the ordering
				for (int col = range.start.col; col <= range.end.col; col++) {
					for (int row = range.end.row; row >= range.start.row; row--) {
						auto i = Vec2Int(col, row);
						Vec2Int n = Vec2Int(col, row + 1);
						Vec2Int s = Vec2Int(col, row - 1);
						Vec2Int e = Vec2Int(col + 1, row);
						Vec2Int w = Vec2Int(col - 1, row);

						auto cell = cells.getRef(i);

						if (!has(i)) continue;

						if (!has(w)) {
							if (cells.has(n) && cells.get(n).edgeExists[WEST]) {
								edges[cells.get(n).edgeId[WEST]].start[1] -= m_cellSize[1];
								cell->edgeId[WEST] = cells.get(n).edgeId[WEST];
								cell->edgeExists[WEST] = true;
							}
							else {
								Edge edge;
								edge.start = Vec2(col * m_cellSize[0] - m_cellSize[0] * 0.5, row * m_cellSize[1] - m_cellSize[1] * 0.5);
								edge.end = Vec2(edge.start[0], edge.start[1] + m_cellSize[1]);
								cell->edgeId[WEST] = edges.size();
								cell->edgeExists[WEST] = true;
								edges.push_back(edge);
							}
						}

						if (!has(e)) {
							if (has(n) && cells.get(n).edgeExists[EAST]) {
								edges[cells.get(n).edgeId[EAST]].end[1] -= m_cellSize[1];
								cell->edgeId[EAST] = cells.get(n).edgeId[EAST];
								cell->edgeExists[EAST] = true;
							}
							else {
								Edge edge;
								edge.start = Vec2((col + 1) * m_cellSize[0] - m_cellSize[0] * 0.5, (row + 1) * m_cellSize[1] - m_cellSize[1] * 0.5);
								edge.end = Vec2(edge.start[0], edge.start[1] - m_cellSize[1]);
								cell->edgeId[EAST] = edges.size();
								cell->edgeExists[EAST] = true;
								edges.push_back(edge);
							}
						}

						if (!has(n)) {
							if (has(w) && cells.get(w).edgeExists[NORTH]) {
								edges[cells.get(w).edgeId[NORTH]].end[0] += m_cellSize[0];
								cell->edgeId[NORTH] = cells.get(w).edgeId[NORTH];
								cell->edgeExists[NORTH] = true;
							}
							else {
								Edge edge;
								edge.start = Vec2((col)*m_cellSize[0] - m_cellSize[0] * 0.5, (row + 1) * m_cellSize[1] - m_cellSize[1] * 0.5);
								edge.end = Vec2(edge.start[0] + m_cellSize[0], edge.start[1]);
								cell->edgeId[NORTH] = edges.size();
								cell->edgeExists[NORTH] = true;
								edges.push_back(edge);
							}
						}
						
						if (!has(s)) {
							if (has(w) && cells.get(w).edgeExists[SOUTH]) {
								edges[cells.get(w).edgeId[SOUTH]].start[0] += m_cellSize[0];
								cell->edgeId[SOUTH] = cells.get(w).edgeId[SOUTH];
								cell->edgeExists[SOUTH] = true;
							}
							else {
								Edge edge;
								edge.start = Vec2((col + 1)*m_cellSize[0] - m_cellSize[0] * 0.5, (row) * m_cellSize[1] - m_cellSize[1] * 0.5);
								edge.end = Vec2(edge.start[0] - m_cellSize[0], edge.start[1]);
								cell->edgeId[SOUTH] = edges.size();
								cell->edgeExists[SOUTH] = true;
								edges.push_back(edge);
							}
						}
						
					}
				}

				return edges;
			}

		private:

			Vec2 m_cellSize;

		};
	}
}

#endif
