#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H
#include "Aliases.h";

namespace hagame {
	namespace utils {
		template <class T>
		class ObjectManager {
		private:
			size_t _objectCount;
			Array<T> _objects;
		};
	}
}

#endif