#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER
#include "../Utils/FileSystem.h"
#include "../Utils/Aliases.h"
#include "../Graphics/ShaderProgram.h"
#include "../Graphics/Texture.h"

namespace hagame {
	class ResourceManager {
	private:
		hagame::utils::FileSystem fs;

		Map<String, Ptr<hagame::graphics::ShaderProgram>> shaderPrograms;
		Map<String, Ptr<hagame::graphics::Texture>> textures;
		Map<String, Ptr<hagame::utils::File>> files;

	public:
		ResourceManager(String basePath);

		Ptr<hagame::utils::File> loadFile(String fileName, String rawFileName);
		Ptr<hagame::utils::File> getFile(String fileName);

		Ptr<hagame::graphics::ShaderProgram> loadShaderProgram(String programName, String vertPath, String fragPath);
		Ptr<hagame::graphics::ShaderProgram> getShaderProgram(String programName);

		Ptr<hagame::graphics::Texture> loadTexture(String textureName, String path);
		Ptr<hagame::graphics::Texture> getTexture(String textureName);
	};
}

#endif