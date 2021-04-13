#ifndef RENDERER_H
#define RENDERER_H
#include "../../HaGame/HaGame.h"

struct MaterialRenderer {
	hagame::graphics::ShaderProgram* shader;
	hagame::graphics::Material material;
	hagame::Transform transform;
	hagame::graphics::Mesh* mesh;
	bool displayBounds;
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
			r.mesh->draw(r.shader);

			if (r.displayBounds) {
				auto shader = game->resources.getShaderProgram("color");
				shader->use();
				shader->setMVP(Mat4::Identity(), viewMat, projMat);
				r.transform.rotate(Quat(dt, Vec3({ 1, 1, 1 })));
				auto cube = transformBoundingBox(r.mesh->getBoundingCube(), r.transform.getModelMatrix().resize<3, 3>(), r.transform.position);
				hagame::graphics::drawCubeOutline(cube, hagame::graphics::Color::purple(), game->resources.getShaderProgram("color"));
			}
		}
		
	}
};


#endif
