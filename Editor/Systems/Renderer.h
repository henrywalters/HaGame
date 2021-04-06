#ifndef RENDERER_H
#define RENDERER_H
#include "../../HaGame/HaGame.h"

struct MaterialRenderer {
	hagame::graphics::ShaderProgram* shader;
	hagame::graphics::Material material;
	hagame::Transform transform;
	hagame::graphics::Mesh mesh;
};

class Renderer : public hagame::ecs::System {
public:
	void onSystemUpdate(double dt) {
		auto view = registry->view<MaterialRenderer>();

		auto viewMat = scene->activeCamera->getViewMatrix();
		auto projMat = scene->activeCamera->getProjMatrix();

		for (auto entity : view) {
			auto& r = view.get<MaterialRenderer>(entity);
			r.shader->use();
			r.shader->setMVP(r.transform.getModelMatrix(), viewMat, projMat);
			r.shader->setMaterial("material", r.material);
			r.mesh.draw(r.shader);
		}
	}
};


#endif
