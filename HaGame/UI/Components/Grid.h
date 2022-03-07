#ifndef UI_GRID_H
#define UI_GRID_H

#include "./../../Utils/Aliases.h"
#include "./../../Graphics/Routines.h"
#include "./UIComponent.h"
#include "./../Common/Range.h"
#include "./../Common/Anchor.h"

namespace hagame {
	namespace ui {

		class Grid : public UIComponent {
		public:

			AnchorType anchor = AnchorType::Center;
			float padding = 0;
			bool display = false;

			Grid(Vec2 size, int cols, int rows) :
				m_rect(Rect(Vec2::Zero(), size)),
				m_cols(cols),
				m_rows(rows),
				m_cellSize(Vec2(m_rect.size[0] / cols, m_rect.size[1] / rows)),
				m_entities(Map<int, Map<int, Ptr<UIEntity>>>()),
				m_grid(Map<int, Map<int, Ptr<Grid>>>())
			{
			}

			Grid(Grid* parent, Vec2 size, int cols, int rows) :
				m_parent(parent),
				m_rect(Rect(Vec2::Zero(), size)),
				m_cols(cols),
				m_rows(rows),
				m_cellSize(Vec2(m_rect.size[0] / cols, m_rect.size[1] / rows)),
				m_entities(Map<int, Map<int, Ptr<UIEntity>>>()),
				m_grid(Map<int, Map<int, Ptr<Grid>>>())
			{
			}

			Rect getBoundingBox() {
				return m_rect;
			}

			void update(Vec2 pos) {
				m_rect.pos = pos;

				if (display) {

					hagame::graphics::drawRect(m_rect, hagame::graphics::Color::green(), hagame::graphics::DEBUG_SHADER, 1);

					for (int i = 1; i < m_rows; i++) {
						hagame::graphics::drawLine(hagame::math::Line(m_rect.pos + Vec2(0, i * m_cellSize[1]), m_rect.pos + Vec2(m_rect.size[0], i * m_cellSize[1])), hagame::graphics::Color::green(), hagame::graphics::DEBUG_SHADER, 1);
					}

					for (int i = 1; i < m_cols; i++) {
						hagame::graphics::drawLine(hagame::math::Line(m_rect.pos + Vec2(i * m_cellSize[0], 0), m_rect.pos + Vec2(i * m_cellSize[0], m_rect.size[1])), hagame::graphics::Color::green(), hagame::graphics::DEBUG_SHADER, 1);
					}
				}

				for (auto& [colIdx, col] : m_grid) {
					for (auto& [rowIdx, grid] : m_grid[colIdx]) {
						grid->update(m_rect.pos + getCell(colIdx, rowIdx).pos);
					}
				}

				for (auto& [colIdx, col] : m_entities) {
					for (auto& [rowIdx, entity] : m_entities[colIdx]) {
						auto cell = getCell(colIdx, rowIdx);
						auto bb = entity->uiFn(entity->entity)->getBoundingBox();

						auto anchorType = m_anchors.find(colIdx) != m_anchors.end() && m_anchors[colIdx].find(rowIdx) != m_anchors[colIdx].end() ? m_anchors[colIdx][rowIdx] : anchor;
						auto cellPadding = m_paddings.find(colIdx) != m_paddings.end() && m_paddings[colIdx].find(rowIdx) != m_paddings[colIdx].end() ? m_paddings[colIdx][rowIdx] : padding;

						auto pos = calculateAnchoredPos(cell, bb, anchorType, cellPadding) + cell.pos;

						if (display) {
							hagame::graphics::drawRect(Rect(pos + m_rect.pos, bb.size), hagame::graphics::Color::blue(), hagame::graphics::DEBUG_SHADER, 1);
						}

						entity->entity->setPos(m_rect.pos + pos);
					}
				}

			}

			void setCols(int cols) {
				m_cols = cols;
				m_cellSize = Vec2(m_rect.size[0] / m_cols, m_rect.size[1] / m_rows);
			}

			void setRows(int rows) {
				m_rows = rows;
				m_cellSize = Vec2(m_rect.size[0] / m_cols, m_rect.size[1] / m_rows);
			}

			void setSize(Vec2 size) {
				m_rect = Rect(Vec2::Zero(), size);
				m_cellSize = Vec2(m_rect.size[0] / m_cols, m_rect.size[1] / m_rows);
				for (auto& [colIdx, col] : m_grid) {
					for (auto& [rowIdx, grid] : m_grid[colIdx]) {
						grid->setSize(getCell(colIdx, rowIdx).size);
					}
				}
			}

			Rect getGridRect() {
				return m_rect;
			}

			// Return the bounding rectangle of a cell (relative to the grid origin)
			Rect getCell(int col, int row) {
				return Rect(Vec2(col * m_cellSize[0], row * m_cellSize[1]), m_cellSize);
			}

			Rect getCellGlobal(int col, int row) {
				auto cell = getCell(col, row);
				cell.pos += m_rect.pos;
				return cell;
			}

			void addEntity(int col, int row, hagame::ecs::Entity* entity, std::function<UIComponent* (hagame::ecs::Entity*)> getUI) {

				if (m_entities.find(col) == m_entities.end()) {
					m_entities.insert(std::make_pair(col, Map<int, Ptr<UIEntity>>()));
				}

				if (m_entities[col].find(row) != m_entities[col].end()) {
					throw new std::exception("Entity already at this grid index");
				}

				m_entities[col].insert(std::make_pair(row, std::make_shared<UIEntity>(entity, getUI)));
			}

			Grid* addGrid(int col, int row, int gridCols, int gridRows) {

				if (m_grid.find(col) == m_grid.end()) {
					m_grid.insert(std::make_pair(col, Map<int, Ptr<Grid>>()));
				}

				if (m_grid[col].find(row) != m_grid[col].end()) {
					throw new std::exception("Grid already at this grid index");
				}

				m_grid[col].insert(std::make_pair(row, std::make_shared<Grid>(this, getCell(col, row).size, gridCols, gridRows)));

				return m_grid[col][row].get();
			}

			void setAnchor(int col, int row, AnchorType _anchor) {
				if (m_anchors.find(col) == m_anchors.end()) {
					m_anchors.insert(std::make_pair(col, Map<int, AnchorType>()));
				}

				m_anchors[col][row] = _anchor;
			}

			void setPadding(int col, int row, float _padding) {
				if (m_paddings.find(col) == m_paddings.end()) {
					m_paddings.insert(std::make_pair(col, Map<int, float>()));
				}
				m_paddings[col][row] = _padding;
			}

			hagame::ecs::Entity* getEntity(int col, int row) {
				return m_entities[col][row]->entity;
			}

			Grid* getGrid(int col, int row) {
				return m_grid[col][row].get();
			}

		private:

			Grid* m_parent;

			Rect m_rect;
			int m_rows, m_cols;

			Vec2 m_cellSize;

			Map<int, Map<int, float>> m_paddings;
			Map<int, Map<int, AnchorType>> m_anchors;
			Map<int, Map<int, Ptr<Grid>>> m_grid;
			Map<int, Map<int, Ptr<UIEntity>>> m_entities;

			Map<String, float> m_rangePaddings;


		};
	}
}

#endif
