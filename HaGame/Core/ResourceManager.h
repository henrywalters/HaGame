#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER
#include "../Utils/FileSystem.h"
#include "../Utils/Aliases.h"
#include "../Graphics/ShaderProgram.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Cubemap.h"
#include "../Graphics/Image.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Font.h"

namespace hagame {
	class ResourceManager {
	private:
		Ptr<hagame::utils::FileSystem> fs;

		Map<String, Ptr<hagame::graphics::ShaderProgram>> shaderPrograms;
		Map<String, Ptr<hagame::graphics::Texture>> textures;
		Map<String, Ptr<hagame::graphics::Cubemap>> cubemaps;
		Map<String, Ptr<hagame::graphics::Image>> images;
		Map<String, Ptr<hagame::graphics::Mesh>> meshes;
		Map<String, Ptr<hagame::utils::File>> files;
		Map<String, Ptr<hagame::graphics::Font>> fonts;

		// Perform preprocessing steps on a shaders source code. For now, this just means resolving #include statements
		String processShaderFile(hagame::utils::File file);


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

		hagame::graphics::Cubemap* loadCubemap(String cubemapName, String paths[6]);
		hagame::graphics::Cubemap* getCubemap(String cubemapName);

		hagame::graphics::Image* loadImage(String imageName, String path, hagame::graphics::ImageType type);
		hagame::graphics::Image* getImage(String imageName);

		// Create a mesh from an OBJ file
		hagame::graphics::Mesh* loadMesh(String meshName, String path);

		// Create a mesh from it's vertices and indicies (typically a primitive will exist!)
		hagame::graphics::Mesh* loadMesh(String meshName, hagame::graphics::MeshDefinition definition);

		// Create a mesh from a combination of many meshes. This will cull shared faces.
		hagame::graphics::Mesh* loadMesh(String meshName, Array<hagame::graphics::ConcatMeshDefinition> definitions);

		hagame::graphics::Mesh* getMesh(String meshName);

		hagame::graphics::Font* loadFont(String fontName, String filePath, int fontSize = 24);
		hagame::graphics::Font* loadWindowsFont(String fontName, int fontSize = 24);
		hagame::graphics::Font* loadWindowsFont(String fontName, String fontType = "Regular", int fontSize = 24);


		hagame::graphics::Font* getFont(String fontName);
	};
}

#endif