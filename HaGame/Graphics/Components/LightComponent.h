#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "../Light.h"

namespace hagame {
	namespace graphics {
		struct LightComponent {
			Light light;
		};

		struct DirLightComponent {
			DirLight light;
		};

		struct PointLightComponent {
			PointLight light;
		};
	}
}

#endif
