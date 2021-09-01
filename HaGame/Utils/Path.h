#ifndef PATH_H
#define PATH_H

#include "../Utils/Aliases.h"
#include "../Utils/String.h"
#include "./FileSystem.h"

#ifdef WIN32
#define PATH_DELIMITER '\\'
#else
#define PATH_DELIMITER '/'
#endif

namespace hagame {
	namespace utils {

		// Manage a path in a simpler way than dealing with raw strings
		class Path {

			Array<String> parts;

		public:
			
			Path(String path) : parts(stringSplit(path, PATH_DELIMITER)) {}
			Path(Array<String> pathParts) : parts(pathParts) {}
			Path() : parts({}) {}

			String join(String delim = String(1, PATH_DELIMITER)) {
				return stringJoin(parts, delim);
			}

			String toString() {
				return join();
			}

			bool isRelativePath() {
				return parts.size() > 0 && parts[0] == "..";
			}

			void push(String part) {
				parts.push_back(part);
			}

			void pop() {
				if (parts.size() > 0)
					parts.pop_back();
			}

			Path copy() {
				Path out = Path();
				for (auto part : parts) {
					out.push(part);
				}
				return out;
			}

			String getFullPath(String root = FileSystem::GetWorkingDirectory()) {
				if (isRelativePath()) {
					Path rootPath = Path(root);
					for (auto part : parts) {
						if (part == "..") {
							rootPath.pop();
						}
						else {
							rootPath.push(part);
						}
					}
					return rootPath.toString();
				}
				else {
					return toString();
				}
			}

			void toAbsolute(String root = FileSystem::GetWorkingDirectory()) {
				if (isRelativePath()) {
					parts = stringSplit(getFullPath(root), PATH_DELIMITER);
				}
			}

			String getBaseName() {
				int size = parts.size();
				return size > 0 ? parts[size - 1] : "";
			}

			String getWorkspace() {
				auto workspacePath = copy();
				workspacePath.pop();
				return workspacePath.toString();
			}
		};
	}
}

#endif
