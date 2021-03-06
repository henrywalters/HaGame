#ifndef PARTICLE
#define PARTICLE
#include "Color.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Particle {
		public:
			Ptr<Texture> texture;
			Vec2 position, velocity;
			Color color;
			float life;

			Particle(Ptr<Texture> _texture): texture(_texture), position(Vec2(0.0f)), velocity(Vec2(0.0f)), color(Color::white()), life(0.0f) {}
		};
	}
}

#endif