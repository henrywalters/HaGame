#include "ResourceManager.h"

hagame::ResourceManager::ResourceManager(String path) {
	fs = std::make_shared<hagame::utils::FileSystem>(path);
}

void hagame::ResourceManager::setBasePath(String basePath)
{
	fs->basePath = basePath;
}

hagame::utils::FileSystem* hagame::ResourceManager::getFileSystem()
{
	return fs.get();
}

hagame::utils::File* hagame::ResourceManager::loadFile(String fileName, String rawFileName)
{
	auto file = std::make_shared<hagame::utils::File>(fs->getFile(rawFileName));
	files[fileName] = file;
	return files[fileName].get();
}

hagame::utils::File* hagame::ResourceManager::getFile(String fileName)
{
	if (!hasKey(files, fileName)) {
		throw new Exception("File does not exist");
	}

	return files[fileName].get();
}

hagame::graphics::ShaderProgram* hagame::ResourceManager::loadShaderProgram(String shaderName)
{
	return loadShaderProgram(shaderName, shaderName + "_vert.glsl", shaderName + "_frag.glsl");
}

hagame::graphics::ShaderProgram* hagame::ResourceManager::loadShaderProgram(String programName, String vertPath, String fragPath) {
	std::string vertSrc = fs->readFile(vertPath);
	std::string fragSrc = fs->readFile(fragPath);
	hagame::graphics::Shader vertShader = hagame::graphics::Shader::LoadVertex(vertSrc);
	hagame::graphics::Shader fragShader = hagame::graphics::Shader::LoadFragment(fragSrc);
	Ptr<hagame::graphics::ShaderProgram> program = std::make_shared<hagame::graphics::ShaderProgram>(vertShader, fragShader);
	shaderPrograms[programName] = program;
	return shaderPrograms[programName].get();
}

hagame::graphics::ShaderProgram* hagame::ResourceManager::getShaderProgram(String programName)
{
	if (!hasKey(shaderPrograms, programName)) {
		throw new Exception("Shader program does not exist");
	}

	return shaderPrograms[programName].get();
}

hagame::graphics::Texture* hagame::ResourceManager::loadTexture(String textureName, String path)
{
	Ptr<hagame::graphics::Texture> texture = std::make_shared<hagame::graphics::Texture>(fs->getFullPath(path));
	textures[textureName] = texture;
	return textures[textureName].get();
}

hagame::graphics::Texture* hagame::ResourceManager::getTexture(String textureName)
{
	if (!hasKey(textures, textureName)) {
		throw new Exception("Texture does not exist");
	}

	return textures[textureName].get();
}

hagame::graphics::Image* hagame::ResourceManager::loadImage(String imageName, String path, hagame::graphics::ImageType type)
{
	Ptr<hagame::graphics::Image> image = std::make_shared<hagame::graphics::Image>(fs->getFullPath(path), type);
	images[imageName] = image;
	return images[imageName].get();
}

hagame::graphics::Image* hagame::ResourceManager::getImage(String imageName)
{
	if (!hasKey(images, imageName)) {
		throw new Exception("Image does not exist");
	}

	return images[imageName].get();
}

hagame::graphics::Mesh* hagame::ResourceManager::loadMesh(String meshName, String path)
{
	Ptr<hagame::graphics::Mesh> mesh = std::make_shared<hagame::graphics::Mesh>(&fs->getFile(path));
	meshes[meshName] = mesh;
	return meshes[meshName].get();
}

hagame::graphics::Mesh* hagame::ResourceManager::loadMesh(String meshName, hagame::graphics::MeshDefinition definition)
{
	Ptr<hagame::graphics::Mesh> mesh = std::make_shared<hagame::graphics::Mesh>(definition);
	meshes[meshName] = mesh;
	return meshes[meshName].get();
}

hagame::graphics::Mesh* hagame::ResourceManager::getMesh(String meshName)
{
	if (!hasKey(meshes, meshName)) {
		throw new Exception("Mesh does not exist");
	}

	return meshes[meshName].get();
}
