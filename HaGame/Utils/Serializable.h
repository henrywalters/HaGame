#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>

namespace hagame {
	namespace utils {
		class Serializable {
		public:
			virtual void deserialize(std::string data) = 0;
			virtual std::string serialize() = 0;
		};
	}
}

#endif
