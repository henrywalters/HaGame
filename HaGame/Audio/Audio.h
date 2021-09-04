#ifndef AUDIO_H
#define AUDIO_H

#include "./../../HaGame/Vendor/soloud/soloud.h"
#include "./Sample.h"

namespace hagame {
	namespace audio {
		class Audio {
			Ptr<SoLoud::Soloud> soloud;
		public:
			void initialize() {
				soloud = std::make_shared<SoLoud::Soloud>();
				soloud->init();
			}

			void play(Sample* sample) {
				soloud->play(sample->wav);
			}
		};
	}
}

#endif