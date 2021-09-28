#ifndef COMPONENT_H
#define COMPONENT_H

#include "./../../Utils/Aliases.h"
#include "./../../Vendor/entt/entt.hpp"

namespace hagame {
	namespace ecs {
		struct Component : entt::type_list<> {
			template<typename Base>
			struct type : Base {
				String getName() {
					return this->template invoke<0>(*this);
				}
			};

			template<typename Type>
			using impl = entt::value_list<&Type::getName>;
		};
	}
}

#endif
