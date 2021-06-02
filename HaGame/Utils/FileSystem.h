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
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


namespace hagame {
	namespace utils {
		class FileSystem {
		public:

			std::string basePath;

			FileSystem();
			FileSystem(std::string _basePath);

			static std::string GetWorkingDirectory();

			std::string GetRelativePath(std::string path);

			std::string getFullPath(std::string path);

			File getFile(std::string path);
			std::string readFile(std::string path);
			std::vector<std::string> listFiles(std::string path);

			void forEachFile(std::string path, std::function<void(std::string filePath)> lambda);
			void forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName)> lambda);
			void forEachFile(std::string path, std::function<void(std::string filePath, std::string fileName, std::string extension)> lambda);
			
			bool isFile(std::string path);
			bool isDir(std::string path);
		};
	}
}

#endif