#include "Collisions.h"

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkLineAgainstPlane(Line line, Plane plane, float& t)
{
	Optional<hagame::math::collisions::Hit> hit;

	Vec3 delta = line.b - line.a;
	float denom = dot(plane.normal, delta);

	if (denom == 0.0f) {
		// Line and plane are parallel so return nothing
		return std::nullopt;
	}

	t = (plane.distance - dot(plane.normal, line.a)) / denom;

	Vec3 point = line.getPointOnLine(t);

	hit = Hit{
		point,
		delta.normalized(),
		0.0f
	};

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstSphere(Ray ray, Sphere sphere, float& t)
{
	std::optional<hagame::math::collisions::Hit> hit;

	Vec3 f = ray.origin - sphere.center;
	float a = dot(ray.direction, ray.direction);
	float b = 2 * dot(f, ray.direction);
	float c = dot(f, f) - sphere.radius * sphere.radius;

	float disc = b * b - 4 * a * c;

	if (disc < 0) {
		return std::nullopt;
	}
	else {
		disc = sqrt(disc);
		float t1 = (-b - disc) / (2 * a);
		float t2 = (-b + disc) / (2 * a);

		if (t1 >= 0 && t1 <= 1) {
			t = t1;
		}
		else if (t2 >= 0 && t2 <= 1) {
			t = t2;
		}
		else {
			return std::nullopt;
		}

		Vec3 pos = ray.getPointOnLine(t);
		Vec3 norm = (pos - sphere.center).normalized();

		hit = hagame::math::collisions::Hit{ pos, norm, (ray.origin + ray.direction - pos).magnitude() };

		return hit;
	}
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstCube(Ray ray, Cube cube, float& t)
{
	const uint8_t RIGHT = 0;
	const uint8_t LEFT = 1;
	const uint8_t MIDDLE = 2;

	std::optional<Hit> hit = std::nullopt;

	bool inside = true;
	int quadrant[3];
	int whichPlane;
	float maxT[3];
	Vec3 cubeMin = cube.min();
	Vec3 cubeMax = cube.max();
	float candidatePlane[3];

	int i;

	for (i = 0; i < 3; i++) {
		if (ray.origin[i] < cubeMin[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = cubeMin[i];
			inside = false;
		}
		else if (ray.origin[i] > cubeMax[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = cubeMax[i];
			inside = false;
		}
		else {
			quadrant[i] = MIDDLE;
		}
	}

	if (inside) {
		t = 0;
		hit = Hit{ Vec3::Zero(), Vec3::Zero() };
		return hit;
	}

	for (i = 0; i < 3; i++)
		maxT[i] = quadrant[i] != MIDDLE && ray.direction[i] != 0.0f ? (candidatePlane[i] - ray.origin[i]) / ray.direction[i] : -1.0f;

	whichPlane = 0;
	for (i = 1; i < 3; i++) {
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;
	}

	if (maxT[whichPlane] < 0.0f || maxT[whichPlane] > 1.0f) return std::nullopt;

	for (i = 0; i < 3; i++) {
		if (whichPlane != i) {
			float pos = ray.origin[i] + maxT[whichPlane] * ray.direction[i];
			if (pos < cubeMin[i] || pos > cubeMax[i])
				return std::nullopt;
		}
	}

	Vec3 norm;
	norm[whichPlane] = quadrant[whichPlane] == 1 ? -1.0f : 1.0f;

	t = maxT[whichPlane];
	auto pos = ray.getPointOnLine(t);
	hit = Hit{ pos, norm, (ray.getPointOnLine(1.0f) - pos).magnitude() };
	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstCapsule(Ray ray, Capsule capsule, float& t)
{
	return Optional<Hit>();
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstTriangle(Ray ray, Triangle tri, float& t)
{
	Optional<hagame::math::collisions::Hit> hit;

	auto planeIntersect = checkRayAgainstPlane(ray, Plane(tri.calcNormal(), tri.a), t);

	if (planeIntersect.has_value() && tri.containsPoint(planeIntersect.value().position))
		hit = planeIntersect;

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstPlane(Ray ray, Plane plane, float& t)
{
	Optional<hagame::math::collisions::Hit> hit;

	t = (plane.distance - dot(plane.normal, ray.origin)) / dot(plane.normal, ray.direction);

	if (t >= 0.0f && t <= 1.0f) {
		Vec3 point = ray.getPointOnLine(t);
		hit = Hit{
			point,
			ray.direction.normalized() * -1,
			ray.getPointOnLine(1.0f - t).magnitude()
		};
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstDisc(Ray ray, Disc disc, float& t)
{
	Optional<hagame::math::collisions::Hit> hit;

	auto intersection = checkRayAgainstPlane(ray, Plane(disc.normal, disc.center), t);

	if (intersection.has_value()) {
		auto delta = intersection.value().position - disc.center;
		if (dot(delta, delta) <= disc.radius * disc.radius) {
			hit = intersection;
		}
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkRayAgainstHollowDisc(Ray ray, Vec3 center, Vec3 normal, float innerRadius, float outerRadius, float& t)
{
	float tmpT;
	auto innerHit = checkRayAgainstDisc(ray, Disc(center, normal, innerRadius), tmpT);
	auto outerHit = checkRayAgainstDisc(ray, Disc(center, normal, outerRadius), tmpT);

	if (outerHit.has_value() && !innerHit.has_value()) {
		t = tmpT;
		return outerHit;
	}

	return std::nullopt;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkAABBAgainstAABB(math::AABB A, math::AABB B)
{
	if (!A.isIntersecting(B))
		return std::nullopt;

	auto closestPointOnB = B.closestPoint(A.center);
	auto closestPointOnA = A.closestPoint(B.center);
	auto delta = closestPointOnB - closestPointOnA;

	return Hit{
		closestPointOnB,
		delta.normalized(),
		delta.magnitude()
	};
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkSphereAgainstLineSegment(Sphere sphere, LineSegment line)
{
	Optional<hagame::math::collisions::Hit> hit = std::nullopt;
	float t;
	Vec3 closestPoint = line.closestPoint(sphere.center, t);
	Vec3 delta = sphere.center - closestPoint;
	float distSq = dot(delta, delta);

	if (distSq <= sphere.radius * sphere.radius) {
		hit = { closestPoint, delta.normalized(), sphere.radius - delta.magnitude() };
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkSphereAgainstSphere(Sphere A, Sphere B)
{
	Optional<hagame::math::collisions::Hit> hit = std::nullopt;

	auto delta = A.center - B.center;
	float deltaMag = delta.magnitude();
	float radialSum = A.radius + B.radius;

	if (deltaMag <= radialSum) {
		float t = B.radius / deltaMag;
		Vec3 pos = B.center + delta * t;
		Vec3 normal = delta.normalized();
		hit = Hit{ pos, normal, A.radius - (pos - A.center).magnitude() };
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkSphereAgainstAABB(Sphere sphere, math::AABB box)
{
	Optional<hagame::math::collisions::Hit> hit;

	auto closestPointOnBox = box.closestPoint(sphere.center);

	if (dot2(closestPointOnBox - sphere.center) > sphere.radius * sphere.radius)
		return std::nullopt;

	auto closestPointOnSphere = sphere.closestPoint(box.center);

	auto delta = closestPointOnBox - closestPointOnSphere;

	return Hit{
		closestPointOnSphere,
		delta.normalized(),
		delta.magnitude()
	};

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkSphereAgainstTriangle(Sphere sphere, Triangle tri)
{
	Optional<hagame::math::collisions::Hit> hit;

	Vec3 normal = tri.calcNormal();
	float dist = dot(sphere.center - tri.a, normal);

	if (std::abs(dist) > sphere.radius)
		return std::nullopt;

	Vec3 projSphereCenter = sphere.center - normal * dist;

	if (tri.containsPoint(projSphereCenter)) {
		auto delta = projSphereCenter - sphere.center;
		hit = { projSphereCenter, normal * -sign(dot(normal, delta)), sphere.radius - delta.magnitude() };
	}
	else {
		Optional<hagame::math::collisions::Hit> tmpHit;
		float bestDist;

		tmpHit = checkSphereAgainstLineSegment(sphere, hagame::math::LineSegment(tri.a, tri.b));
		
		if (tmpHit.has_value()) {
			hit = tmpHit.value();
			bestDist = tmpHit.value().depth;
		}

		tmpHit = checkSphereAgainstLineSegment(sphere, hagame::math::LineSegment(tri.b, tri.c));

		if (tmpHit.has_value() && tmpHit.value().depth < bestDist) {
			hit = tmpHit.value();
			bestDist = tmpHit.value().depth;
		}

		tmpHit = checkSphereAgainstLineSegment(sphere, hagame::math::LineSegment(tri.c, tri.a));

		if (tmpHit.has_value() && tmpHit.value().depth < bestDist) {
			hit = tmpHit.value();
			bestDist = tmpHit.value().depth;
		}
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkSphereAgainstSurface(Sphere sphere, Surface surface, Mat4 transform)
{
	Optional<hagame::math::collisions::Hit> hit;

	for (auto tri : surface.triangles) {
		auto tmpHit = checkSphereAgainstTriangle(sphere, tri.transformed(transform));
		if (tmpHit.has_value() && (!hit.has_value() || hit.value().depth > tmpHit.value().depth))
			hit = tmpHit;
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkCapsuleAgainstCapsule(Capsule A, Capsule B)
{
	Optional<hagame::math::collisions::Hit> hit;
	Vec3 bestA, bestB;

	/*Vec3 aNorm = normalize(A.center.b - A.center.b);
	Vec3 aLineOffset = aNorm * A.radius;
	Vec3 a1 = A.center.a + aLineOffset;
	Vec3 a2 = A.center.b - aLineOffset;

	Vec3 bNorm = normalize(B.center.b - B.center.b);
	Vec3 bLineOffset = bNorm * B.radius;
	Vec3 b1 = B.center.a + bLineOffset;
	Vec3 b2 = B.center.b - bLineOffset;*/

	Vec3 v0 = B.center.a - A.center.a;
	Vec3 v1 = B.center.b - A.center.a;
	Vec3 v2 = B.center.a - A.center.b;
	Vec3 v3 = B.center.b - A.center.b;

	float d0 = dot(v0, v0);
	float d1 = dot(v1, v1);
	float d2 = dot(v2, v2);
	float d3 = dot(v3, v3);

	float t;

	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
		bestA = A.center.b;
	else
		bestA = A.center.a;

	bestB = B.center.closestPoint(bestA, t);
	bestA = A.center.closestPoint(bestB, t);

	Vec3 normal = bestA - bestB;
	float len = normal.magnitude();
	float depth = A.radius + B.radius - len;
	normal.normalize();
	if (depth > 0) {
		hit = Hit{ bestB + (B.radius - depth) * normal, normal, depth };
	}

	return hit;
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkCapsuleAgainstTriangle(Capsule capsule, Triangle tri)
{
	Optional<hagame::math::collisions::Hit> hit;

	Vec3 capNorm = normalize(capsule.center.b - capsule.center.a);
	Vec3 capOffset = capNorm * capsule.radius;

	Vec3 triNorm = tri.calcNormal();

	float t;
	Vec3 refPoint;

	auto linePlaneHit = checkLineAgainstPlane(Line(capsule.center.a, capsule.center.b), Plane(triNorm, dot(triNorm, tri.a)), t);

	if (!linePlaneHit.has_value()) {
		refPoint = tri.b;
	}
	else {
		
		auto linePlaneIntersect = linePlaneHit.value().position;

		if (DEBUG_DISPLAY) {
			hagame::graphics::drawSphereOutline(linePlaneIntersect, 0.05f, hagame::graphics::Color::red(), DEBUG_SHADER);
		}

		if (tri.containsPoint(linePlaneIntersect)) {
			refPoint = linePlaneIntersect;
		}
		else {

			Vec3 p1 = LineSegment(tri.a, tri.b).closestPoint(linePlaneIntersect, t);
			Vec3 v1 = linePlaneIntersect - p1;
			float distSq = dot(v1, v1);
			float bestDist = distSq;
			refPoint = p1;

			if (DEBUG_DISPLAY) {
				hagame::graphics::drawSphereOutline(p1, 0.05f, hagame::graphics::Color::blue(), DEBUG_SHADER);
			}

			Vec3 p2 = LineSegment(tri.b, tri.c).closestPoint(linePlaneIntersect, t);
			Vec3 v2 = linePlaneIntersect - p2;
			if (DEBUG_DISPLAY) {
				hagame::graphics::drawSphereOutline(p2, 0.05f, hagame::graphics::Color::green(), DEBUG_SHADER);
			}
			distSq = dot(v2, v2);
			if (distSq < bestDist) {
				bestDist = distSq;
				refPoint = p2;

			}

			Vec3 p3 = LineSegment(tri.c, tri.a).closestPoint(linePlaneIntersect, t);
			Vec3 v3 = linePlaneIntersect - p3;
			if (DEBUG_DISPLAY) {
				hagame::graphics::drawSphereOutline(p3, 0.05f, hagame::graphics::Color::purple(), DEBUG_SHADER);
			}
			distSq = dot(v3, v3);
			if (distSq < bestDist) {
				bestDist = distSq;
				refPoint = p3;

				
			}
		}
	}

	Vec3 center = capsule.center.closestPoint(refPoint, t);

	return checkSphereAgainstTriangle(Sphere(center, capsule.radius), tri);
}

Optional<hagame::math::collisions::Hit> hagame::math::collisions::checkCapsuleAgainstSurface(Capsule A, Surface surface, Mat4 transform)
{
	Optional<hagame::math::collisions::Hit> hit;

	for (auto tri : surface.triangles) {
		auto tmpHit = checkCapsuleAgainstTriangle(A, tri.transformed(transform));
		if (tmpHit.has_value() && (!hit.has_value() || hit.value().depth > tmpHit.value().depth))
			hit = tmpHit;
	}

	return hit;
}
