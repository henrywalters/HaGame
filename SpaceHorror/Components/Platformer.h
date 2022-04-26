#ifndef PLATFORMER_H
#define PLATFORMER_H

struct Platformer {
	float groundDrag = 700.0f;
	float airDrag = 5.0f;
	bool grounded = false;

	void updateImGui() {
		ImGui::DragFloat("Ground Drag", &groundDrag, 5.0f, 0.0f, 10000.0f);
		ImGui::DragFloat("Air Drag", &airDrag, 5.0f, 0.0f, 10000.0f);
		ImGui::Checkbox("Grounded", &grounded);
	}
};

#endif