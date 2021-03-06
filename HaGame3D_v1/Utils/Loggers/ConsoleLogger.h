#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include "../Logger.h"
#include <string>
#include <iostream>

namespace hagame {
	namespace utils {
		namespace loggers {
			class ConsoleLogger : public Logger {
			public:
				void log(std::string message) {
					std::cout << message << std::endl;
				}
			};
		}
	}
}

#endif
