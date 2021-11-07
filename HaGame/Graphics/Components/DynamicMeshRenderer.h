#ifndef DYNAMIC_MESH_RENDERER_H
#define DYNAMIC_MESH_RENDERER_H

#include "../DynamicMesh.hpp"
#include "../ShaderProgram.h"
#include "../Material.h"
#include "../Texture.h"
#include "../Color.h"

namespace hagame {
	namespace graphics {
		struct DynamicMeshRenderer {
			hagame::graphics::ShaderProgram* shader;
			hagame::graphics::Material material;
			hagame::graphics::Color color;
			Ptr<hagame::graphics::DynamicMesh> mesh;
			hagame::graphics::Texture* texture;
		};
	}
}

#endif
