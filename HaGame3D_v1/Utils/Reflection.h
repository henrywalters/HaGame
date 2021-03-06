#ifndef REFLECTION_H
#define REFLECTION_H

#include <string>
#include <typeinfo>

namespace hagame {
	namespace utils {
		class Reflection {
		public:
			template <class T>
			static std::string GetClassName() {
				return typeid(T).name();
			}
		};
	}
}

#endif