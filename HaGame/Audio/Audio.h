#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "./Sample.h"

namespace hagame {
	namespace audio {
		class Audio {
		public:
			static void Initialize() {
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					throw new std::exception("Failed to initialize SDL2 mixer");
				}
			}
		};
	}
}

#endif