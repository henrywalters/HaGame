#ifndef LIGHT_H
#define LIGHT_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		struct Light {
			Vec3 ambient;
			Vec3 diffuse;
			Vec3 specular;

			Light() : ambient(Vec3::Identity()), diffuse(Vec3::Identity()), specular(Vec3::Identity()) {}
		};

		struct DirLight {
			Vec3 direction;
			Vec3 ambient;
			Vec3 diffuse;
			Vec3 specular;

			DirLight() : direction(Vec3::Top()), ambient(Vec3::Identity()), diffuse(Vec3::Identity()), specular(Vec3::Identity()) {}
		};

		struct PointLight {
			Vec3 position;
			Vec3 ambient;
			Vec3 diffuse;
			Vec3 specular;

			float constant;
			float linear;
			float quadratic;

			PointLight() : position(Vec3::Zero()), 
				ambient(Vec3::Identity()), 
				diffuse(Vec3::Identity()), 
				specular(Vec3::Identity()),
				constant(1.0f),
				linear(0.01f),
				quadratic(0.001f)
			{}
		};

		struct SpotLight {
			Vec3 position;
			Vec3 direction;
			float cutOff;
		};
	}
}

#endif
