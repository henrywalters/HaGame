#ifndef FILE_SYSTEM
#define FILE_SYSTEM
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <iostream>
#include "File.h"
#include "String.h"

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#include <string.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


namespace hagame {
	namespace utils {

		bool DEFAULT_FILE_SORT(std::string a, std::string b) {
			return a < b;
		}

		class FileSystem {
		public:

			std::string basePath;

			FileSystem();
			FileSystem(std::string _basePath);

			static std::string GetWorkingDirectory();

			std::string GetRelativePath(std::string path);

			std::string getFullPath(std::string path);

			File getFile(std::string path);
			File getFileOrCreate(std::string path);
			File getGlobalFile(std::string path);
			std::string readFile(std::string path);
			std::vector<std::string> listAll(std::string path, std::function<bool(std::string fileA, std::string fileB)> compFn = DEFAULT_FILE_SORT);
			std::vector<std::string> listFiles(std::string path, std::function<bool(std::string fileA, std::string fileB)> compFn = DEFAULT_FILE_SORT);
			std::vector<std::string> listDirs(std::string path, std::function<bool(std::string fileA, std::string fileB)> compFn = DEFAULT_FILE_SORT);

			File getFileDialog();

			void forEachFile(std::string path, std::function<void(std::string filePath)> lambda);
			void forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName)> lambda);
			void forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName, std::string extension)> lambda);
			
			bool isGlobalFile(std::string path);
			bool isFile(std::string path);
			bool isDir(std::string path);

			bool containsExtension(std::string ext, std::string path = "");

			void goToParent();

			static std::string GetFileName(std::string path);
			static std::string GetFile(std::string path);
			static std::string GetFileExtension(std::string path);

		};
	}
}

#endif