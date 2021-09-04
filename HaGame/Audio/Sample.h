#ifndef AUDIO_SAMPLE_H
#define AUDIO_SAMPLE_H

#include "../Vendor/soloud/soloud.h"
#include "../Vendor/soloud/soloud_wav.h"

namespace hagame {
	namespace audio {

		class Audio;

		class Sample {

			friend class Audio;

			SoLoud::Wav wav;
			
		public:
			Sample(String wavFile) {
				wav.load(wavFile.c_str());
			}
		};
	}
}

#endif
