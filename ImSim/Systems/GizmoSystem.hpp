#ifndef GIZMO_SYSTEM_HPP
#define GIZMO_SYSTEM_HPP

#include "./../../HaGame/HaGame.h"
#include "./MouseSystem.hpp"
#include "./StateSystem.hpp"
#include "./../Components/Gizmo.hpp"

using namespace hagame::ecs;
using namespace hagame::graphics;

class GizmoSystem : public hagame::ecs::System {

	float SIZE_FACTOR = 0.2f;
	float SCALE_FACTOR = 0.15f;
	float DRAG_FACTOR = 0.15f;
	float ROT_FACTOR = 0.15f;

	ShaderProgram* shader;
	Ptr<MouseSystem> mouseSystem;
	Ptr<StateSystem> stateSystem;
	Ptr<Entity> selectedEntity;

	int selectedAxis = -1;
	int adjustingAxis = -1;
	Vec3 initialAdjustPos;
	Vec3 initialEntityAdjustPos;
	Vec3 initialEntityScale;
	Quat initialEntityRot;

	float getSize(Ptr<Entity> entity) {
		auto delta = entity->transform->getPosition() - scene->activeCameraEntity->transform->getPosition();
		return delta.magnitude() * SIZE_FACTOR;
	}

	void setMeshBorder(Ptr<Entity> entity, bool bordered) {
		auto meshRenderer = entity->getComponent<DynamicMeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->displayBorder = bordered;
			meshRenderer->borderColor = Color("#ff7a14");
			meshRenderer->borderShader = shader;
		}
	}

	void selectEntity(Ptr<Entity> entity) {

		while (entity->parent != NULL) {
			entity = entity->parent;
		}

		if (selectedEntity != NULL) {
			selectedEntity->getComponent<Gizmo>()->active = false;
			setMeshBorder(selectedEntity, false);
		}

		selectedEntity = entity;

		if (selectedEntity->hasComponent<Gizmo>()) {
			selectedEntity->getComponent<Gizmo>()->active = true;
		}
		else {
			auto gizmo = selectedEntity->addComponent<Gizmo>();
			gizmo->type = Gizmo::Type::Movement;
		}

		setMeshBorder(selectedEntity, true);
	}

	int checkAxisCollisions(Ptr<Entity> entity, float thickness, float length) {
		float t;
		std::optional<hagame::math::Ray::Hit> hit;
		Cube cubeX = Cube(entity->transform->getPosition() - Vec3({ 0.0f, thickness / 2.0f, thickness / 2.0f }), Vec3({length, thickness, thickness}));
		Cube cubeY = Cube(entity->transform->getPosition() - Vec3({ thickness / 2.0f, 0.0f, thickness / 2.0f }), Vec3({ thickness, length, thickness }));
		Cube cubeZ = Cube(entity->transform->getPosition() - Vec3({ thickness / 2.0f, thickness / 2.0f, 0.0f }), Vec3({ thickness, thickness, length }));

		if (hagame::math::collisions::checkRayAgainstCube(mouseSystem->mouseWorldRay, cubeX, t).has_value())
			return 0;
		else if (hagame::math::collisions::checkRayAgainstCube(mouseSystem->mouseWorldRay, cubeY, t).has_value())
			return 1;
		else if (hagame::math::collisions::checkRayAgainstCube(mouseSystem->mouseWorldRay, cubeZ, t).has_value())
			return 2;
		else
			return -1;
	}

	int checkRotationCollisions(Ptr<Entity> entity, float radius, float thickness) {
		float t;
		std::optional<hagame::math::Ray::Hit> hit;
		hagame::math::Disc discX = hagame::math::Disc(entity->transform->getPosition(), Vec3::Right(), radius);
		hagame::math::Disc discY = hagame::math::Disc(entity->transform->getPosition(), Vec3::Top(), radius);
		hagame::math::Disc discZ = hagame::math::Disc(entity->transform->getPosition(), Vec3::Face(), radius);

		float innerRad = radius - thickness / 2.0f;
		float outerRad = radius + thickness / 2.0f;

		if (hagame::math::collisions::checkRayAgainstHollowDisc(mouseSystem->mouseWorldRay, entity->transform->getPosition(), Vec3::Right(), innerRad, outerRad, t).has_value())
			return 0;
		else if (hagame::math::collisions::checkRayAgainstHollowDisc(mouseSystem->mouseWorldRay, entity->transform->getPosition(), Vec3::Top(), innerRad, outerRad, t).has_value())
			return 1;
		else if (hagame::math::collisions::checkRayAgainstHollowDisc(mouseSystem->mouseWorldRay, entity->transform->getPosition(), Vec3::Face(), innerRad, outerRad, t).has_value())
			return 2;
		else
			return -1;
	}

	void handleMovementGizmo(Ptr<Entity> entity, Gizmo* gizmo) {
		float size = getSize(entity);
		selectedAxis = checkAxisCollisions(entity, 0.1, size);
		drawMovementGizmo(entity->transform->getPosition(), size, shader, selectedAxis);
	}

	void handleRotationGizmo(Ptr<Entity> entity, Gizmo* gizmo) {
		float size = getSize(entity);
		selectedAxis = checkRotationCollisions(entity, size, 0.1);
		std::cout << selectedAxis << "\n";
		drawRotationGizmo(entity->transform->getPosition(), size, shader, selectedAxis);
	}

	void handleScaleGizmo(Ptr<Entity> entity, Gizmo* gizmo) {
		float size = getSize(entity);
		selectedAxis = checkAxisCollisions(entity, 0.1, size);
		drawScaleGizmo(entity->transform->getPosition(), size, shader, selectedAxis);
		return;
	}

public:

	String getSystemName() {
		return "Gizmo System";
	}

	void onSystemStart() {
		shader = game->resources->getShaderProgram("color");
		mouseSystem = scene->getSystem<MouseSystem>();
		stateSystem = scene->getSystem<StateSystem>();
	}

	void onSystemUpdate(double dt) {
		shader->use();
		shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);

		forEach<Gizmo>([this, dt](Gizmo* g, Ptr<Entity> entity) {

			if (!g->active)
				return;

			if (game->input.keyboardMouse.keyboard.numbersPressed[1])
				g->type = Gizmo::Type::Movement;
			if (game->input.keyboardMouse.keyboard.numbersPressed[2])
				g->type = Gizmo::Type::Rotation;
			if (game->input.keyboardMouse.keyboard.numbersPressed[3])
				g->type = Gizmo::Type::Scale;

			switch (g->type) {
			case Gizmo::Type::Movement:
				handleMovementGizmo(entity, g);
				break;
			case Gizmo::Type::Rotation:
				handleRotationGizmo(entity, g);
				break;
			case Gizmo::Type::Scale:
				handleScaleGizmo(entity, g);
				break;
			}

			if (selectedAxis != -1 && game->input.keyboardMouse.mouse.leftPressed) {
				adjustingAxis = selectedAxis;
				initialAdjustPos = mouseSystem->mouseWorldPos;
				initialEntityAdjustPos = entity->transform->getPosition();
				initialEntityRot = entity->transform->getRotation();
				initialEntityScale = entity->transform->getScale();
			}
			
			if (!game->input.keyboardMouse.mouse.left) {
				adjustingAxis = -1;
			}

			if (adjustingAxis != -1) {

				auto delta = mouseSystem->mouseWorldPos - initialAdjustPos;

				switch (g->type) {
				case Gizmo::Type::Movement:
				{
					auto pos = initialEntityAdjustPos;
					pos[adjustingAxis] += delta[adjustingAxis] * DRAG_FACTOR;
					entity->transform->setPosition(pos);
				}
				break;
				case Gizmo::Type::Rotation:
				{
					auto rot = initialEntityRot;
					auto axis = Vec3::Zero();
					axis[adjustingAxis] = 1.0f;
					rot = Quat(delta.magnitude() * sign(delta) * ROT_FACTOR, axis) * rot;
					entity->transform->setRotation(rot);
				}
				break;
				case Gizmo::Type::Scale:
				{
					auto scale = initialEntityScale;
					scale[adjustingAxis] += delta.magnitude() * sign(delta) * SCALE_FACTOR;
					entity->transform->setScale(scale);
				}
					break;
				}
			}
		});

		bool debug = stateSystem->state->debug;

		if (selectedAxis == -1) {
			float t;
			auto rayHit = game->collisions.raycast(scene->activeCameraEntity, mouseSystem->mouseWorldRay, t, { "player" });

			if (rayHit.has_value() && debug && game->input.keyboardMouse.mouse.leftPressed) {
				std::cout << rayHit.value().entity->uuid << "\n";
				selectEntity(rayHit.value().entity);
			}
		}

	}
};

#endif
