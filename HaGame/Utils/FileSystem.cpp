#include "FileSystem.h"

hagame::utils::FileSystem::FileSystem() {
	basePath = hagame::utils::FileSystem::GetWorkingDirectory();
}

hagame::utils::FileSystem::FileSystem(std::string _basePath) {
	basePath = _basePath;
}

std::string hagame::utils::FileSystem::GetWorkingDirectory() {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string cwd(buff);
	return cwd;
}

std::string hagame::utils::FileSystem::GetRelativePath (std::string path) {
	return path.substr(basePath.length(), path.length());
}

std::string hagame::utils::FileSystem::getFullPath(std::string path) {
	return basePath + "/" + path;
}

hagame::utils::File hagame::utils::FileSystem::getFile(std::string path)
{
	if (!isFile(path)) {
		throw new std::exception("File does not exist");
	}

	return File(basePath, path);
}

std::string hagame::utils::FileSystem::readFile(std::string path) {
	std::ifstream stream(getFullPath(path));
	return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

std::vector<std::string> hagame::utils::FileSystem::listFiles(std::string path) {
	std::vector<std::string> files = std::vector<std::string>();
	for (const auto& entry : std::filesystem::directory_iterator(getFullPath(path))) {
		files.push_back(hagame::utils::FileSystem::GetRelativePath(entry.path().string()));
	}
	return files;
}

void hagame::utils::FileSystem::forEachFile(std::string path, std::function<void(std::string filePath)> lambda)
{
	for (std::string file : listFiles(path)) {
		lambda(file);
	}
}

void hagame::utils::FileSystem::forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName)> lambda)
{
	for (std::string file : listFiles(path)) {
		auto parts = stringSplit(file, file.find('\\') == String::npos ? '/' : '\\');
		auto name = stringSplit(parts[parts.size() - 1], '.')[0];
		lambda(file, name);
	}
}

void hagame::utils::FileSystem::forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName, std::string extension)> lambda)
{
	for (std::string file : listFiles(path)) {
		auto parts = stringSplit(file, file.find('\\') == String::npos ? '/' : '\\');
		auto fileNameParts = stringSplit(parts[parts.size() - 1], '.');
		lambda(file, fileNameParts[0], fileNameParts[1]);
	}
}

bool hagame::utils::FileSystem::isFile(std::string path) {
	const std::filesystem::path fsPath(getFullPath(path));
	return std::filesystem::is_regular_file(fsPath);
}

bool hagame::utils::FileSystem::isDir(std::string path) {
	const std::filesystem::path fsPath(getFullPath(path));
	return std::filesystem::is_directory(fsPath);
}