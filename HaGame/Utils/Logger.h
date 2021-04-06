#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace hagame {
	namespace utils {
		class Logger {
		public:
			virtual void log(std::string message) {
				throw new std::exception("log not properly implemented");
			}
		};
	}
}

#endif