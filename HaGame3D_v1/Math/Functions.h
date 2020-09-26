#ifndef Functions
#define Functions

template <class T>
T clamp(T val, T min, T max) {
	if (val >= max) return max;
	else if (val <= min) return min;
	else return val;
}

#endif