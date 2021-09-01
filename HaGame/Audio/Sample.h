#ifndef AUDIO_SAMPLE_H
#define AUDIO_SAMPLE_H

#include "./../Utils/Aliases.h"
#include <SDL.h>
#include <SDL_mixer.h>

namespace hagame {
	namespace audio {
		class Sample {
			Mix_Chunk* data = NULL;
			
		public:
			Sample(String wavFile) {
				data = Mix_LoadWAV(wavFile.c_str());
				if (data == NULL) {
					throw new std::exception("Failed to load wav file");
				}
			}

			~Sample() {
				if (data != NULL) {
					Mix_FreeChunk(data);
				}
			}
		};
	}
}

#endif
