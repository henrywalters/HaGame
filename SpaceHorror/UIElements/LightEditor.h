#ifndef LIGHT_EDITOR_H
#define LIGHT_EDITOR_H

#include "../../HaGame/HaGame.h"

struct LightEditor {

	Color color;
	float constant = 1.0;
	float linear = 1.0;
	float quadratic = 1.0;

	void render() {
		ImGui::ColorEdit4("Color", color.vector);
		ImGui::SliderFloat("Constant", &constant, 0, 20);
		ImGui::SliderFloat("Linear", &linear, 0, 100);
		ImGui::SliderFloat("Quadratic", &quadratic, 0, 100);
	}

};

#endif
