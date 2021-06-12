#ifndef OBJECT_H
#define OBJECT_H

#include <string>

namespace hagame {

	// An (experimental) base object class, HaGame classes can extend from to provide basic functionalities
	class Object {
		virtual uint32_t getObjectId() {
			throw new std::exception("getObjectId() is not defined for this class");
		}

		virtual std::string toString() {
			throw new std::exception("toString() is not defined for this class");
		}
	};
}

#endif
