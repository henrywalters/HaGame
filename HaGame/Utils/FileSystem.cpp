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
	// TODO: Fix this and make it cross platform
	return basePath + "\\" + path;
}

hagame::utils::File hagame::utils::FileSystem::getFile(std::string path)
{
	if (!isFile(path)) {
		throw new std::exception("File does not exist");
	}

	return File(basePath, path);
}

hagame::utils::File hagame::utils::FileSystem::getFileOrCreate(std::string path)
{
	return File(basePath, path);
}

hagame::utils::File hagame::utils::FileSystem::getGlobalFile(std::string path)
{
	if (!isGlobalFile(path)) {
		throw new std::exception("Global File does not exist");
	}

	return File("", path);
}

std::string hagame::utils::FileSystem::readFile(std::string path) {
	std::ifstream stream(getFullPath(path));
	return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

std::vector<std::string> hagame::utils::FileSystem::listAll(std::string path, std::function<bool(std::string fileA, std::string fileB)> compFn)
{
	std::vector<std::string> files = std::vector<std::string>();
	std::vector<std::string> dirs = std::vector<std::string>();
	for (const auto& entry : std::filesystem::directory_iterator(getFullPath(path))) {
		auto filePath = hagame::utils::FileSystem::GetRelativePath(entry.path().string());
		if (isDir(filePath))
			dirs.push_back(filePath);
		else
			files.push_back(filePath);
	}

	std::sort(files.begin(), files.end(), compFn);
	std::sort(dirs.begin(), dirs.end(), compFn);

	dirs.insert(dirs.end(), files.begin(), files.end());

	return dirs;
}

std::vector<std::string> hagame::utils::FileSystem::listFiles(std::string path, std::function<bool(std::string fileA, std::string fileB)> compFn) {
	std::vector<std::string> files = std::vector<std::string>();
	for (const auto& entry : std::filesystem::directory_iterator(getFullPath(path))) {
		auto filePath = hagame::utils::FileSystem::GetRelativePath(entry.path().string());
		if (!isDir(filePath))
			files.push_back(filePath);
	}

	std::sort(files.begin(), files.end(), compFn);

	return files;
}

std::vector<std::string> hagame::utils::FileSystem::listDirs(std::string path, std::function<bool(std::string fileA, std::string fileB)> compFn)
{
	std::vector<std::string> dirs = std::vector<std::string>();
	for (const auto& entry : std::filesystem::directory_iterator(getFullPath(path))) {
		auto filePath = hagame::utils::FileSystem::GetRelativePath(entry.path().string());
		if (isDir(filePath))
			dirs.push_back(filePath);
	}

	std::sort(dirs.begin(), dirs.end(), compFn);

	return dirs;
}

void hagame::utils::FileSystem::forEachFile(std::string path, std::function<void(std::string filePath)> lambda)
{
	for (std::string file : listAll(path)) {
		lambda(file);
	}
}

void hagame::utils::FileSystem::forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName)> lambda)
{
	for (std::string file : listAll(path)) {
		lambda(file, GetFile(file));
	}
}

void hagame::utils::FileSystem::forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName, std::string extension)> lambda)
{
	for (std::string file : listAll(path)) {
		auto fileNameParts = stringSplit(GetFileName(file), '.');
		lambda(file, fileNameParts[0], fileNameParts[1]);
	}
}

bool hagame::utils::FileSystem::isGlobalFile(std::string path)
{
	const std::filesystem::path fsPath(path);
	return std::filesystem::is_regular_file(fsPath);
}

bool hagame::utils::FileSystem::isFile(std::string path) {
	const std::filesystem::path fsPath(getFullPath(path));
	return std::filesystem::is_regular_file(fsPath);
}

bool hagame::utils::FileSystem::isDir(std::string path) {
	const std::filesystem::path fsPath(getFullPath(path));
	return std::filesystem::is_directory(fsPath);
}

bool hagame::utils::FileSystem::containsExtension(std::string ext, std::string path)
{
	for (auto file : listAll(path)) {

		if (isDir(file)) {
			bool contains = containsExtension(ext, file);
			if (contains) {
				return true;
			}
		}
		else {
			auto fileParts = stringSplit(file, '.');
			auto fileExt = fileParts[fileParts.size() - 1];

			if (fileExt == ext) {
				return true;
			}
		}
		
	}

	return false;
}

void hagame::utils::FileSystem::goToParent()
{
	auto parts = stringSplit(basePath, '\\');
	if (parts.size() > 1) {
		parts.pop_back();
		basePath = stringJoin(parts, "\\");
	}

}

std::string hagame::utils::FileSystem::GetFile(std::string path)
{
	auto parts = stringSplit(path, path.find('\\') == String::npos ? '/' : '\\');
	auto fileNameParts = stringSplit(parts[parts.size() - 1], '.');
	return fileNameParts[0];
}

std::string hagame::utils::FileSystem::GetFileName(std::string path)
{
	auto parts = stringSplit(path, path.find('\\') == String::npos ? '/' : '\\');
	return parts[parts.size() - 1];
}

std::string hagame::utils::FileSystem::GetFileExtension(std::string path)
{
	auto parts = stringSplit(path, path.find('\\') == String::npos ? '/' : '\\');
	auto fileNameParts = stringSplit(parts[parts.size() - 1], '.');
	return fileNameParts[1];
}
