#ifndef SPATIAL_MAP_H
#define SPATIAL_MAP_H

#include "Aliases.h"
#include "../Math/Ray.h"

namespace hagame {
	namespace utils {
		
		// SpacialMap provides a partition of data over a physical space. E.G. partition game objects with floating points into a 3D grid.
		template <class DataType, class SpatialDataType>
		class SpatialMap {

			const long MAX_CHUNKS = 1000000;

			const long P1 = 73856093;
			const long P2 = 19349663;
			const long P3 = 83492791;

			
		public:
			Map<long, Array<Ptr<DataType>>> map;
			hagame::math::Vector<3, SpatialDataType> chunkSize;

			SpatialMap(hagame::math::Vector<3, SpatialDataType> _chunkSize) : map(Map<long, Array<Ptr<DataType>>>()), chunkSize(_chunkSize) {}

			void clear() {
				map.clear();
			}

			void insert(hagame::math::Vector<3, SpatialDataType> position, Ptr<DataType> value) {
				long hash = hashPosition(position);
				if (map.find(hash) == map.end()) {
					map[hash] = Array<Ptr<DataType>>();
				}
				map[hash].push_back(value);
			}

			Array<Ptr<DataType>> get(hagame::math::Vector<3, SpatialDataType> position) {
				return map[hashPosition(position)];
			}

			Array<Ptr<DataType>> get(hagame::math::Vector<3, SpatialDataType> position, SpatialDataType radius) {
				Array<Ptr<DataType>> output = Array<Ptr<DataType>>();
				Vec3 points[] = {
					Vec3::Zero(),
					Vec3::Top() * radius,
					Vec3::Bottom() * radius,
					Vec3::Right() * radius,
					Vec3::Left() * radius,
					Vec3::Face() * radius,
					Vec3::Back() * radius
				};

				for (int i = 0; i < 7; i++) {
					auto neighbors = map[hashPosition(position + points[i])];
					output.insert(output.end(), neighbors.begin(), neighbors.end());
				}

				return output;
			}

			Array<Ptr<DataType>> get(hagame::math::Vector<3, SpatialDataType> position, hagame::math::Vector<3, SpatialDataType> radius) {
				Array<Ptr<DataType>> output = Array<Ptr<DataType>>();
				Vec3 points[] = {
					Vec3::Top() * radius[1],
					Vec3::Bottom() * radius[1],
					Vec3::Right() * radius[0],
					Vec3::Left() * radius[0],
					Vec3::Face() * radius[2],
					Vec3::Back() * radius[2]
				};

				for (int i = 0; i < 6; i++) {
					auto neighbors = map[hashPosition(position + points[i])];
					output.insert(output.end(), neighbors.begin(), neighbors.end());
				}

				return output;
			}

			void remove(hagame::math::Vector<3, SpatialDataType> position) {
				map.erase(hashPosition(position));
			}

			bool has(hagame::math::Vector<3, SpatialDataType> position) {
				return map.find(hashPosition(position)) != map.end();
			}

			// Hashing function based on "Optimized Spatial Hashing for Collision Detection of Deformable Objects." Read more: http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
			long hashPosition(hagame::math::Vector<3, SpatialDataType> position) {
				return (((int) (position[0] / chunkSize[0]) * P1) ^ ((int)(position[1] / chunkSize[1]) * P2) ^ ((int)(position[2] / chunkSize[2]) * P3)) % MAX_CHUNKS;
			}
		};
	}
}

#endif
