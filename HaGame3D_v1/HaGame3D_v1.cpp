// HaGame3D_v1.cpp : Defines the entry point for the application.
//

#include "HaGame3D_v1.h"

using Vec3 = hagame::math::Vector<3, double>;

int main()
{
	Vec3 vect = Vec3({ 1, 2, 3});
	Vec3 vect2 = Vec3({ 1 / 1.0, 1 / 2.0, 1 / 3.0 });
	Vec3 vect3 = Vec3({ 5, 10, 15 });

	std::cout << vect.toString() << std::endl;
	std::cout << "Size: " << sizeof(Vec3) << std::endl;
	std::cout << "Magnitude: " << vect.magnitude() << std::endl;
	std::cout << "Normalized: " << vect.normalized().toString() << std::endl;
	std::cout << "Original: " << vect.toString() << std::endl;
	std::cout << "Vect 2: " << vect2.toString() << std::endl;
	std::cout << "Prod: " << vect.prod(vect2).toString() << std::endl;
	std::cout << "Dot: " << vect.dot(vect2) << std::endl;
	std::cout << "A + C = " << (vect + vect3).toString() << std::endl;
	std::cout << "A *= 10 = " << (vect *= 10).toString() << std::endl;
	std::cout << "A * 1/10. = " << (vect * 1 / 10.0).toString() << std::endl;
	std::cout << "1/10.0 * A = " << (1 / 10.0 * vect).toString() << std::endl;
	return 0;
}
