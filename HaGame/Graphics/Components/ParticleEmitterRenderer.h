#ifndef PARTICLE_EMITTER_RENDERER
#define PARTICLE_EMITTER_RENDERER

#include "../ParticleEmitter.h"

namespace hagame {
	namespace graphics {
		struct ParticleEmitterRenderer {
			Ptr<ParticleEmitter> emitter;
			ShaderProgram* shader;

			ParticleEmitterRenderer() {
				emitter = std::make_shared<ParticleEmitter>();
			}
		};
	}
}

#endif
