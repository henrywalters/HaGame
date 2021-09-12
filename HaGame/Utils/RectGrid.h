#ifndef RECT_GRID_H
#define RECT_GRID_H

#include "./Aliases.h"

namespace hagame {
	namespace utils {

		// RectGrid provides a data structure for creating nested row / col structures
		class RectGrid {
		public:

			RectGrid(Rect _rect, Vec2Int _partitions) :
				rect(_rect),
				partitions(_partitions) {

				if (_partitions != Vec2Int::Zero())
					split = true;

				auto cellSize = _rect.size.div(_partitions.cast<float>());

				for (int row = 0; row < partitions[1]; row++) {
					Array<Ptr<RectGrid>> rowChildren;
					for (int col = 0; col < partitions[0]; col++) {
						auto cellPos = cellSize.prod(Vec2({ (float)col, (float)row }));
						rowChildren.push_back(std::make_shared<RectGrid>(Rect(cellPos + rect.pos, cellSize), Vec2Int::Zero()));
					}
					children.push_back(rowChildren);
				}
			}

			~RectGrid() {
				for (auto row : children) {
					for (auto child : row) {
						child.reset();
					}
				}
			}

			static Ptr<RectGrid> Columns(Rect _rect, int _cols) {
				return std::make_shared<RectGrid>(_rect, Vec2Int({_cols, 1}));
			}

			static Ptr<RectGrid> Rows(Rect _rect, int _rows) {
				return std::make_shared<RectGrid>(_rect, Vec2Int({ 1, _rows }));
			}

			static Ptr<RectGrid> Grid(Rect _rect, int _rows, int _cols) {
				return std::make_shared<RectGrid>(_rect, Vec2Int({ _cols, _rows }));
			}

			Ptr<RectGrid> getChild(int index) {

				if (partitions[0] != 1 && partitions[1] != 1)
					throw new std::exception("Can only get child with single index for Column / Row partitions");

				if (partitions[1] == 1)
					return getChild(0, index);
				else
					return getChild(index, 0);
			}

			Ptr<RectGrid> getChild(int row, int col) {
				if (!split)
					throw new std::exception("Rect grid has no children yet!");

				return children[row][col];
			}

			Rect getRect() {
				return rect;
			}

			Rect getColumns(int colStart, int colEnd) {
				return getRect(colStart, colEnd, 0, 0);
			}

			Rect getRows(int rowStart, int rowEnd) {
				return getRect(0, 0, rowStart, rowEnd);
			}

			Rect getRect(int colStart, int colEnd, int rowStart, int rowEnd) {
				Rect bounds;
				bool init = false;

				for (int row = rowStart; row <= rowEnd; row++) {
					for (int col = colStart; col <= colEnd; col++) {
						auto child = getChild(row, col);
						auto childRect = child->getRect();
						if (!init) {
							init = true;
							bounds = childRect;
						}
						else {
							if (childRect.pos[0] < bounds.pos[0])
								bounds.pos[0] = childRect.pos[0];
							if (childRect.pos[1] < bounds.pos[1])
								bounds.pos[1] = childRect.pos[1];
							if (childRect.size[0] + childRect.pos[0] > bounds.size[0] + bounds.pos[0])
								bounds.size[0] = childRect.pos[0] + childRect.size[0] - bounds.pos[0];
							if (childRect.size[1] + childRect.pos[1] > bounds.size[1] + bounds.pos[1])
								bounds.size[1] = childRect.pos[1] + childRect.size[1] - bounds.pos[1];
						}
					}
				}

				return bounds;
			}

		private:

			Rect rect;
			bool split = false;
			Vec2Int partitions;
			Array<Array<Ptr<RectGrid>>> children;
		};
	}
}

#endif
