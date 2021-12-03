#ifndef GIZMO_HPP
#define GIZMO_HPP

struct Gizmo {
	enum Type {
		Movement,
		Rotation,
		Scale
	};

	Type type;
	bool allAxis = false;
	int axis = -1;
};

#endif
