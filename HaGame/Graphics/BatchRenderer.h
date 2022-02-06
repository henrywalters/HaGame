#ifndef BATCH_RENDERER_H
#define BATCH_RENDERER_H

#include "./../Utils/Aliases.h"
#include "LineBuffer.h"
#include "MeshBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Color.h"

namespace hagame {
	namespace graphics {

		struct BatchMesh {
			Mesh* mesh;
			ShaderProgram* shader;
			ShaderProgram* borderShader;
			Material material;
			Color color;
			Color borderColor;
			Texture* texture;
			Texture* normal;
			Texture* opacity;
			Texture* specular;
		};

		class BatchRenderer {
		public:
			// Clear all of the buffers
			void clear();

			// Enqueue a mesh to be rendered by the batch renderer.
			void enqueue(BatchMesh mesh, Vec3 pos, Quat rot, Vec3 scale);

			// Render all of the buffers queued up
			void draw();
		private:

			static unsigned int LastMeshBufferId = 0;
			Map<unsigned int, Ptr<MeshBuffer>> m_meshBuffers;

			Map<unsigned int, ShaderProgram*> m_shaders;
			Map<unsigned int, ShaderProgram*> m_borderShaders;
			Map<unsigned int, Material> m_materials;
			Map<unsigned int, Color> m_colors;
			Map<unsigned int, Color> m_borderColors;
			Map<unsigned int, Texture*> m_textures;
			Map<unsigned int, Texture*> m_normal;
			Map<unsigned int, Texture*> m_opacity;
			Map<unsigned int, Texture*> m_specular;

			uint64_t hashMesh(BatchMesh mesh);

		};

	}
}


#endif
