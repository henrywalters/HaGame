#ifndef PLATFORMER_H
#define PLATFORMER_H

struct Platformer {
	float groundDrag = 0.5f;
	float airDrag = 0.1f;
	bool grounded = false;

	void updateImGui() {
		ImGui::DragFloat("Ground Drag", &groundDrag, 0.01, 0.0, 2.0);
		ImGui::DragFloat("Air Drag", &airDrag, 0.01, 0.0, 2.0);
		ImGui::Checkbox("Grounded", &grounded);
	}
};

#endif