#ifndef EDITABLE_H
#define EDITABLE_H

#include "./../HaGame.h"

namespace hagame {
	namespace ui {
		struct Editable {
			virtual void renderUI() = 0;
			virtual Map<String, String> serialize() = 0;
			virtual void deserialize(Map<String, String> data) = 0;
		};
	}
}

#endif