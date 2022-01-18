#ifndef GRID_PARTITION_H
#define GRID_PARTITION_H

#include "./IPartition.h"
#include "./../Utils/SpatialMap.h"

namespace hagame {
	namespace physics {

		Vec3 CHUNK_SIZE = Vec3(5.0f);

		enum class GridInsertMode {
			SinglePoint,
			AllPoints,
		};

		template <class T>
		class GridPartition : public IPartition<T> {

			Ptr<utils::SpatialMap<T, float>> m_grid;

			void insertFromGrid(Array<Ptr<T>> &objects, Vec3 pos) {
				auto neighbors = m_grid->get(pos);
				objects.insert(objects.end(), neighbors.begin(), neighbors.end());
			}

			void insertSurface(hagame::math::Surface surface, Vec3 pos, Ptr<T> object) {
				if (mode == GridInsertMode::AllPoints) {
					for (auto tri : surface.triangles) {
						m_grid->insert(pos + tri.a, object);
						m_grid->insert(pos + tri.b, object);
						m_grid->insert(pos + tri.c, object);
					}
				}
				else {
					for (auto tri : surface.triangles) {
						m_grid->insert(pos + tri.calcCentroid(), object);
					}
				}
			}

			void insertBox(hagame::math::AABB box, Vec3 pos, Ptr<T> object) {
				if (mode == GridInsertMode::AllPoints) {
					m_grid->insert(box.center + Vec3(-box.radius[0], -box.radius[1], box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(-box.radius[0], box.radius[1], box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(box.radius[0], box.radius[1], box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(box.radius[0], -box.radius[1], box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(-box.radius[0], -box.radius[1], -box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(-box.radius[0], box.radius[1], -box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(box.radius[0], box.radius[1], -box.radius[2]) + pos, object);
					m_grid->insert(box.center + Vec3(box.radius[0], -box.radius[1], -box.radius[2]) + pos, object);
				}
				else {
					m_grid->insert(box.getMin() + pos, object);
				}
			}

		public:

			GridInsertMode mode = GridInsertMode::AllPoints;

			GridPartition() {
				m_grid = std::make_shared<utils::SpatialMap<T, float>>(CHUNK_SIZE);
			}

			math::AABB getPartition(Vec3 point) {
				return math::AABB(point.div(CHUNK_SIZE).floor().prod(CHUNK_SIZE) + CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
			}

			void insert(BoundingVolume volume, Vec3 pos, Ptr<T> object) {
				if (volume.getType() == BoundingVolumeType::Surface)
					insertSurface(volume.getSurface(), pos, object);
				else
					insertBox(volume.getBoundingBox(), pos, object);
			}

			void clear() {
				m_grid->clear();
			}

			Array<Ptr<T>> getNeighbors(Vec3 point) {
				return m_grid->get(point);
			}

			Array<Ptr<T>> getNeighbors(BoundingVolume* volume, Vec3 pos) {
				auto box = volume->getBoundingBox();
				box.center += pos;
				auto results = Array<Ptr<T>>();
				if (mode == GridInsertMode::AllPoints) {
					insertFromGrid(results, box.getMin());

					auto minPos = box.getMin();

					auto boxMin = getPartition(minPos);
					auto right = getPartition(minPos + Vec3::Right(CHUNK_SIZE[0]));
					auto top = getPartition(minPos + Vec3::Top(CHUNK_SIZE[1]));
					auto front = getPartition(minPos + Vec3::Face(CHUNK_SIZE[2]));
					auto topRight = getPartition(minPos + Vec3(CHUNK_SIZE[0], CHUNK_SIZE[1], 0));
					auto topFront = getPartition(minPos + Vec3(0, CHUNK_SIZE[1], CHUNK_SIZE[2]));
					auto rightFront = getPartition(minPos + Vec3(CHUNK_SIZE[0], 0, CHUNK_SIZE[2]));
					auto boxMax = getPartition(minPos + Vec3(CHUNK_SIZE));

					if (DEBUG_GRAPHICS)
						hagame::graphics::drawCubeOutline(boxMin.getCube(), Color::red());

					if (box.isIntersecting(right)) {
						insertFromGrid(results, right.center);
						if (DEBUG_GRAPHICS)
							hagame::graphics::drawCubeOutline(right.getCube(), Color::red());
					}

					if (box.isIntersecting(front)) {
						insertFromGrid(results, front.center);
						if (DEBUG_GRAPHICS)
							hagame::graphics::drawCubeOutline(front.getCube(), Color::red());
	
						if (box.isIntersecting(rightFront)) {
							insertFromGrid(results, rightFront.center);
							if (DEBUG_GRAPHICS)
								hagame::graphics::drawCubeOutline(rightFront.getCube(), Color::red());
						}
					}

					if (box.isIntersecting(top)) {
						insertFromGrid(results, top.center);
						if (DEBUG_GRAPHICS)
							hagame::graphics::drawCubeOutline(top.getCube(), Color::red());

						if (box.isIntersecting(boxMax)) {
							insertFromGrid(results, boxMax.center);
							if (DEBUG_GRAPHICS)
								hagame::graphics::drawCubeOutline(boxMax.getCube(), Color::red());
						}

						if (box.isIntersecting(topFront)) {
							insertFromGrid(results, topFront.center);
							if (DEBUG_GRAPHICS)
								hagame::graphics::drawCubeOutline(topFront.getCube(), Color::red());
						}

						if (box.isIntersecting(rightFront)) {
							insertFromGrid(results, rightFront.center);
							if (DEBUG_GRAPHICS)
								hagame::graphics::drawCubeOutline(rightFront.getCube(), Color::red());
						}
					}
				}
				else {
					return {};
				}


				return results;
			}

		};
	}
}

#endif
