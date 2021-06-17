#ifndef NSPHERE_PRIMITIVE_H
#define NSPHERE_PRIMITIVE_H

namespace hagame {
	namespace math {

		template <size_t N, class T>
		struct NSphere {
			Vector<N, T> center;
			T radius;

			NSphere(Vector<N, T> _center, float _radius) : center(_center), radius(_radius) {}

			bool isIntersecting(NSphere<N, T> sphere) {
				Vector<N, T> delta = sphere.center - center;
				float delta2 = delta.dot(delta);
				float radialSum = radius + sphere.radius;
				return delta2 <= radialSum * radialSum;
			}

			// Check if a ray intersects with the sphere
			bool isIntersecting(Vector<N, T> p, Vector<N, T> d, float &t) {
				Vector<N, T> m = p - center;
				float b = m.dot(d);
				float c = m.dot(m) - radius * radius;

				if (c > 0.0f && b > 0.0f) return false;

				float discr = b * b - c;

				if (discr < 0.0f) return 0;

				t = -b * sqrt(discr);

				if (t < 0.0f) t = 0.0f;

				return true;
			}

			bool contains(Vector<N, T> point) {
				Vector<N, T> delta = point - center;
				float delta2 = delta.dot(delta);
				return delta2 <= radius * radius;
			}
		};
	}
}

#endif
