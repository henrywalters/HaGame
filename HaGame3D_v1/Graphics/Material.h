#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Material {
		public:
			Vec3 ambient;
			Vec3 diffuse;
			Vec3 specular;
			float shininess;

			Material() {
				ambient = Vec3::Zero();
				diffuse = Vec3::Zero();
				specular = Vec3::Zero();
				shininess = 0;
			}

			Material(Vec3 _ambient, Vec3 _diffuse, Vec3 _specular, float _shininess) : ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess) {}

			static Material emerald() {
				return Material(
					Vec3({0.0215, 0.1745, 0.0215}),
					Vec3({ 0.07568, 0.61424, 0.07568 }),
					Vec3({ 0.633, 0.727811, 0.633 }),
					0.6
				);
			}

			static Material obsidian() {
				return Material(
					Vec3({ 0.05375, 0.05, 0.06625 }),
					Vec3({ 0.18275, 0.17, 0.22525 }),
					Vec3({ 0.332741, 0.328634, 0.346435 }),
					0.3
				);
			}

			static Material chrome() {
				return Material(
					Vec3({ 0.25, 0.25, 0.25 }),
					Vec3({ 0.4, 0.4, 0.4 }),
					Vec3({ 0.774597, 0.774597, 0.774597 }),
					0.6
				);
			}
		};
	}
}

#endif
