#ifndef UI_RANGE_H
#define UI_RANGE_H

#include "./../../Utils/Aliases.h"

namespace hagame {
	namespace ui {
		struct Range {
			Vec2Int minimum;
			Vec2Int maximum;

			Range():
				minimum(Vec2Int::Zero()),
				maximum(Vec2Int::Zero())
			{}

			Range(Vec2Int _max) :
				minimum(Vec2Int::Zero()),
				maximum(_max)
			{}

			Range(Vec2Int _min, Vec2Int _max) :
				minimum(_min),
				maximum(_max)
			{}

			String toHash() {
				return minimum.toString() + "_" + maximum.toString();
			}
		};
	}
}

#endif
