#ifndef PIXEL_H
#define PIXEL_H

#include "./Color.h"
#include "./../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Pixel {
		public:
			Vec2 pos;
			Color color;
		};
	}
}

#endif