#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER
#include "../Utils/FileSystem.h"
#include "../Utils/Aliases.h"
#include "../Graphics/ShaderProgram.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Image.h"
#include "../Graphics/Mesh.h"

namespace hagame {
	class ResourceManager {
	private:
		hagame::utils::FileSystem fs;

		Map<String, Ptr<hagame::graphics::ShaderProgram>> shaderPrograms;
		Map<String, Ptr<hagame::graphics::Texture>> textures;
		Map<String, Ptr<hagame::graphics::Image>> images;
		Map<String, Ptr<hagame::graphics::Mesh>> meshes;
		Map<String, Ptr<hagame::utils::File>> files;

	public:
		ResourceManager(String basePath);

		void setBasePath(String basePath);

		hagame::utils::FileSystem* getFileSystem();

		hagame::utils::File* loadFile(String fileName, String rawFileName);
		hagame::utils::File* getFile(String fileName);

		// Create a shader program from files that match fs_path/{shaderName}_vert.glsl and fs_path/{shaderName}_frag.glsl
		hagame::graphics::ShaderProgram* loadShaderProgram(String shaderName);

		// Create a shader program from individually specified vertex and fragment paths
		hagame::graphics::ShaderProgram* loadShaderProgram(String programName, String vertPath, String fragPath);

		hagame::graphics::ShaderProgram* getShaderProgram(String programName);

		hagame::graphics::Texture* loadTexture(String textureName, String path);
		hagame::graphics::Texture* getTexture(String textureName);

		hagame::graphics::Image* loadImage(String imageName, String path, hagame::graphics::ImageType type);
		hagame::graphics::Image* getImage(String imageName);

		// Create a mesh from an OBJ file
		hagame::graphics::Mesh* loadMesh(String meshName, String path);
		hagame::graphics::Mesh* getMesh(String meshName);
	};
}

#endif