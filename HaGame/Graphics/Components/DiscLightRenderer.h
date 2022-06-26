#ifndef DISC_LIGHT_RENDERER_H
#define DISC_LIGHT_RENDERER_H

#include "../Disc.hpp"
#include "../Color.h"

namespace hagame {
	namespace graphics {
		struct DiscLightRenderer {
			ShaderProgram* shader;
			Ptr<Disc> disc;
			Color color;
			float constant = 1.0;
			float linear = 1.0;
			float quadratic = 1.0;

			void renderUI() {
				ImGui::ColorEdit4("Color", color.vector);
				ImGui::SliderFloat("Constant", &constant, 0.0, 10.0);
				ImGui::SliderFloat("Linear", &linear, 0.0, 10.0);
				ImGui::SliderFloat("Quadratic", &quadratic, 0.0, 10.0);
			}
		};
	}
}

#endif
