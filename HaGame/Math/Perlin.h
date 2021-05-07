#ifndef PERLIN_H
#define PERLIN_H

#include "Functions.h"
#include "../Utils/Random.h"

namespace hagame {
	namespace math {

		template <size_t M, size_t N, class T>
		class Perlin {
		private:

			
			utils::Random random;

			Vec2 randomGradient(Vec2Int idx) {
				T random = 2920.f * sin(idx[0] * 21942.0 + idx[1] * 171324.0 + 8912.0) * cos(idx[0] * 23157.0 * idx[1] * 217832.0 + 9758.0);
				return Vector<2, T>({ cos(random), sin(random) });
			}

			T dotGridGradient(Vec2Int idx, Vector<2, T>  pos) {
				Vector<2, T>  grad = randomGradient(idx);
				Vector<2, T>  delta = pos - idx.cast<float>();
				return delta.dot(grad);
			}

		public:

			int permutations[M * N];
			T gradient[M * N];

			Perlin() {
				for (int i = 0; i < M * N; i++) {
					permutations[i] = i;
				}

				for (int i = 0; i < M * N; i++) {
					int r = random.integer<unsigned int>(0, M * N - 1);
					int a = permutations[i];
					int b = permutations[r];
					permutations[i] = b;
					permutations[r] = a;
				}
			}

			T compute(Vector<2, T> pos) {
				Vec2Int idx0 = Vec2Int({ static_cast<int>(pos[0]), static_cast<int>(pos[1]) });
				Vec2Int idx1 = idx0 + Vec2Int::Identity();

				Vector<2, T>  s = pos - idx0.cast<float>();

				T n0, n1, ix0, ix1, value;

				n0 = dotGridGradient(idx0, pos);
				n1 = dotGridGradient(Vec2Int({ idx1[0], idx0[1] }), pos);
				ix0 = lerp(n0, n1, s[0]);

				n0 = dotGridGradient(Vec2Int({ idx1[0], idx0[1] }), pos);
				n1 = dotGridGradient(idx1, pos);
				ix1 = lerp<T>(n0, n1, s[0]);

				return lerp<T>(ix0, ix1, s[1]);
			}
		};
	}
}

#endif