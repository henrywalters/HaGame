#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "../Mesh.h"
#include "../ShaderProgram.h"
#include "../Material.h"
#include "../Texture.h"
#include "../Color.h"

namespace hagame {
	namespace graphics {
		struct MeshRenderer {
			hagame::graphics::ShaderProgram* shader;
			hagame::graphics::Material material;
			hagame::graphics::Color color;
			hagame::graphics::Mesh* mesh;
			hagame::graphics::Texture* texture;
			hagame::graphics::Texture* normalMap;
			hagame::graphics::Texture* opacityTexture;
			hagame::graphics::Texture* specularTexture;
		};
	}
}

#endif
