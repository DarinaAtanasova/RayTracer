#ifndef VEC3_H
#define VEC3_H

#include<cmath>
#include<iostream>

using std::sqrt;

inline double random_double();
inline double random_double(double, double);

class vec3 {
	public: 
		double e[3];

	public:
		vec3() : e{ 0, 0, 0 } {}
		vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

		double operator[](int i) const { return e[i]; }
		
		double& operator[](int i) {	return e[i]; }

		vec3& operator+= (const vec3& v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}

		vec3& operator*= (const double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		vec3& operator/= (const double t) {
			return *this *= 1 / t;
		}

		double length_squared() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

		double length() const {
			return sqrt(length_squared());
		}

		inline static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}

		inline static vec3 random(double min, double max) {
			return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

		// Return true if the vector is close to zero in all dimensions.
		bool near_zero() const {
			const auto s = 1e-8; //10^-8
			//fabs() - absolute value
			return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s); 
		}
};

using point3 = vec3;
using color = vec3;

//Vec3 Utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3 &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
	return v * t;
}

inline vec3 operator/ (const vec3 v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3& v1, const vec3& v2) {
	return (v1.e[0] * v2.e[0]) + (v1.e[1] * v2.e[1]) + (v1.e[2] * v2.e[2]);
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(v1[1] * v2[2] - v1[2] * v2[1],
		v1[0] * v2[2] - v1[2] * v2[0],
		v1[0] * v2[1] - v1[1] * v2[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline vec3 random_in_unit_sphere() {
	while (true) {
		vec3 random_p = vec3::random(-1, 1);
		if (random_p.length_squared() >= 1) {
			// p > 1 - outside sphere
			// p = 1 - on the sphere
			// p < 1 - inside sphere
			continue;
		}
		return random_p;
	}
}

vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

#endif // !VEC3_H
