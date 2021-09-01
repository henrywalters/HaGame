#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "./../Utils/Aliases.h"

namespace hagame {
	namespace graphics {

		struct Particle {
			Vec3 pos;
			Vec4 color;
		};

		enum EmitterShape {
			PointEmitter,
			SphereEmitter,
			BoxEmitter,
			TaurusEmitter,
		};

		struct EmitterSettings {
			// If this is set to true, the emitter will never stop
			bool infiniteLifeTime = true;

			// If infiniteLife is set to false, the emitter will emit for this long
			float lifeTime = 0.1f;

			// The delay between each emission
			float delay = 0.001f;

			// How many particles to emit per emission
			int particlesEmitted = 500;

			// Particles will be emitted from this shape
			EmitterShape shape = EmitterShape::PointEmitter;

			// Required for SphereEmitter
			Optional<float> radius = std::nullopt;

			// Required for BoxEmitter
			Optional<Vec3> boxSize = std::nullopt;

			// Required for Taurus Emtitter
			Optional<float> innerRadius = std::nullopt;

			// Required for Taurus Emtitter
			Optional<float> outerRadius = std::nullopt;

			// If the emitter shape is a square, this must be defined
		};

		struct ParticleSettings {

		};

		class ParticleEmitter {

		};
	}
}

#endif
