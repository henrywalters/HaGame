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

			String toHash() const {
				return minimum.toString() + "_" + maximum.toString();
			}

			bool operator==(const Range& range) const {
				return range.minimum == minimum && range.maximum == maximum;
			}

			struct Hash {
				size_t operator()(const Range& range) const {
					return std::hash<std::string_view>{}(range.toHash());
				}
			};
		};
	}
}

#endif
