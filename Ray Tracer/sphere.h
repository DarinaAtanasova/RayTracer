#ifndef SPHERE_H
#define SPHERE_H 

#include "hittable.h"
#include "vec3.h"

//sphere inherits hittable
class sphere : public hittable {
	public:
		point3 center;
		double radius;
		shared_ptr<material> mat_ptr;

	public:
		sphere(){}
		sphere(point3 center, double radius, shared_ptr<material> m) : 
			center(center), radius(radius), mat_ptr(m) {};

		virtual bool hit(ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(ray& ray, double t_min, double t_max, hit_record& rec) const {
	vec3 ac = ray.origin() - center; //A-C

	//dot product(A, A) = (vector length)^2

	auto a = ray.direction().length_squared(); // b^2 //dot(ray.direction(), ray.direction())
	auto half_b = dot(ray.direction(), ac); //b(A-C)
	auto c = ac.length_squared() - radius * radius; //(A-C)^2-r^2 //dot(ac, ac)
	
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) {
		return false;
	}
	auto sqrt_D = sqrt(discriminant);

	// Find the nearest root which lies in range: 
	// t_min < root < t_max
	auto root = (-half_b - sqrt_D) / a;
	if (root < t_min || root > t_max) {
		root = (-half_b + sqrt_D) / a;
		if (root < t_min || root > t_max) {
			return false;
		}
	}

	rec.t = root;
	rec.p = ray.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(ray, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}

#endif // !SPHERE_H
