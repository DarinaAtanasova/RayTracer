#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Common Headers
#include "ray.h"
#include "vec3.h"

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;


//Utility functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180;
}

inline double random_double() {
	//Random double in [0, 1)
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max) {
	//Random double in [min, max]
	static std::uniform_real_distribution<double> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double clamp(double x, double min, double max) {
	if (x < min) {
		return min;
	}
	if (x > max) {
		return max;
	}
	return x;
}

struct Vertex {
	float x, y, z;
};

struct Triangle {
	int v0, v1, v2;
};

// Cast a Vertex to point3
point3 make_point(Vertex v) {
	point3 p;
	p.e[0] = v.x;
	p.e[1] = v.y;
	p.e[2] = v.z;
	return p;
}

#endif // !UTILITY_FUNCTIONS_H
