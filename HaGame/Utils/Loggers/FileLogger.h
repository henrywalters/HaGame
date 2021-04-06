#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include "../Logger.h"
#include "../File.h"
#include <string>
#include <iostream>

namespace hagame {
	namespace utils {
		namespace loggers {
			class FileLogger : public Logger {

			private:

				File* _file;

			public:

				FileLogger(File* file): _file(file) {}

				void log(std::string message) {
					_file->append(message + "\n");
				}
			};
		}
	}
}

#endif
