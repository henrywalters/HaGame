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
					Vec3({ 0.0215, 0.1745, 0.0215 }),
					Vec3({ 0.07568, 0.61424, 0.07568 }),
					Vec3({ 0.633, 0.727811, 0.633 }),
					0.6
				);
			}

			static Material jade() {
				return Material(
					Vec3({ 0.135, 0.2225, 0.1575 }),
					Vec3({ 0.54, 0.89, 0.63 }),
					Vec3({ 0.316228, 0.316228, 0.316228 }),
					0.1
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

			static Material pearl() {
				return Material(
					Vec3({ 0.25, 0.20725, 0.20725 }),
					Vec3({ 1, 0.829, 0.829 }),
					Vec3({ 0.296648, 0.296648, 0.296648 }),
					0.088
				);
			}

			static Material ruby() {
				return Material(
					Vec3({ 0.1745, 0.01175, 0.01175 }),
					Vec3({ 0.61424, 0.04136, 0.04136 }),
					Vec3({ 0.727811, 0.626959, 0.626959 }),
					0.6
				);
			}

			static Material turquoise() {
				return Material(
					Vec3({ 0.1, 0.18725, 0.1745 }),
					Vec3({ 0.396, 0.74151, 0.69102 }),
					Vec3({ 0.297254, 0.30829, 0.306678 }),
					0.1
				);
			}

			static Material brass() {
				return Material(
					Vec3({ 0.329412, 0.223529, 0.027451 }),
					Vec3({ 0.780392, 0.568627, 0.113725 }),
					Vec3({ 0.992157, 0.941176, 0.807843 }),
					0.21794872
				);
			}

			static Material bronze() {
				return Material(
					Vec3({ 0.2125, 0.1275, 0.054 }),
					Vec3({ 0.714, 0.4284, 0.18144 }),
					Vec3({ 0.393548, 0.271906, 0.166721 }),
					0.2
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

			static Material copper() {
				return Material(
					Vec3({ 0.19125, 0.0735, 0.0225 }),
					Vec3({ 0.7038, 0.27048, 0.0828 }),
					Vec3({ 0.256777, 0.137622, 0.086014 }),
					0.1
				);
			}

			static Material gold() {
				return Material(
					Vec3({ 0.24725, 0.1995, 0.0745 }),
					Vec3({ 0.75164, 0.60648, 0.22648 }),
					Vec3({ 0.628281, 0.555802, 0.366065 }),
					0.4
				);
			}

			static Material silver() {
				return Material(
					Vec3({ 0.19225, 0.19225, 0.19225 }),
					Vec3({ 0.50754, 0.50754, 0.50754 }),
					Vec3({ 0.508273, 0.508273, 0.508273 }),
					0.4
				);
			}

			static Material blackPlastic() {
				return Material(
					Vec3({ 0.0, 0.0, 0.0 }),
					Vec3({ 0.01, 0.01, 0.01 }),
					Vec3({ 0.50, 0.50, 0.50 }),
					.25
				);
			}

			static Material cyanPlastic() {
				return Material(
					Vec3({ 0.0, 0.1, 0.06 }),
					Vec3({ 0.0, 0.50980392, 0.50980392 }),
					Vec3({ 0.50196078, 0.50196078, 0.50196078 }),
					.25
				);
			}

			static Material greenPlastic() {
				return Material(
					Vec3({ 0.0, 0.0, 0.0 }),
					Vec3({ 0.1, 0.35, 0.1 }),
					Vec3({ 0.45, 0.55, 0.45 }),
					.25
				);
			}

			static Material redPlastic() {
				return Material(
					Vec3({ 0.0, 0.0, 0.0 }),
					Vec3({ 0.5, 0.0, 0.0 }),
					Vec3({ 0.7, 0.6, 0.6 }),
					.25
				);
			}

			static Material whitePlastic() {
				return Material(
					Vec3({ 0.0, 0.0, 0.0 }),
					Vec3({ 0.55, 0.55, 0.55 }),
					Vec3({ 0.70, 0.70, 0.70 }),
					.25
				);
			}

			static Material yellowPlastic() {
				return Material(
					Vec3({ 0.0, 0.0, 0.0 }),
					Vec3({ 0.5, 0.5, 0.0 }),
					Vec3({ 0.60, 0.60, 0.50 }),
					.25
				);
			}

			static Material blackRubber() {
				return Material(
					Vec3({ 0.02, 0.02, 0.02 }),
					Vec3({ 0.01, 0.01, 0.01 }),
					Vec3({ 0.4, 0.4, 0.4 }),
					.078125
				);
			}

			static Material cyanRubber() {
				return Material(
					Vec3({ 0.0, 0.05, 0.05 }),
					Vec3({ 0.4, 0.5, 0.5 }),
					Vec3({ 0.04, 0.7, 0.7 }),
					.078125
				);
			}

			static Material greenRubber() {
				return Material(
					Vec3({ 0.0, 0.05, 0.0 }),
					Vec3({ 0.4, 0.5, 0.4 }),
					Vec3({ 0.04, 0.7, 0.04 }),
					.078125
				);
			}

			static Material redRubber() {
				return Material(
					Vec3({ 0.05, 0.0, 0.0 }),
					Vec3({ 0.5, 0.4, 0.4 }),
					Vec3({ 0.7, 0.04, 0.04 }),
					.078125
				);
			}

			static Material whiteRubber() {
				return Material(
					Vec3({ 0.05, 0.05, 0.05 }),
					Vec3({ 0.5, 0.5, 0.5 }),
					Vec3({ 0.7, 0.7, 0.7 }),
					.078125
				);
			}

			static Material yellowRubber() {
				return Material(
					Vec3({ 0.05, 0.05, 0.0 }),
					Vec3({ 0.5, 0.5, 0.4 }),
					Vec3({ 0.7, 0.7, 0.04 }),
					.078125
				);
			}


		};
	}
}

#endif
