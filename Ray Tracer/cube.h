#ifndef CUBE_H
#define CUBE_H

#include "hittable.h"
#include "vec3.h"

class cube : public hittable {
	public:
		point3 center;
		double half_side;
		shared_ptr<material> mat_ptr;

	private:
		bool hit_side(
			double target, double start, double dir, const ray& ray, const vec3& normal, hit_record& rec) const;

	public:
		//cube() {};
		cube(point3 center, double a, shared_ptr<material> m) :
			center(center), half_side(a), mat_ptr(m) {};

		virtual bool hit(ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool cube::hit_side(double target, double start, double dir, const ray& ray, const vec3& normal, hit_record& rec) const{
	if (start > target && dir >= 0) {
		return false;
	}
	if (start < target && dir <= 0) {
		return false;
	}

	double scale_factor = (target - start) / dir;
	point3 ip = ray.orig + ray.dir * scale_factor;

	if ((ip.x() > center.x() + half_side + 1e-6) ||
		(ip.x() < center.x() - half_side - 1e-6)) {
		return false;
	}
	if ((ip.y() > center.y() + half_side + 1e-6) ||
		(ip.y() < center.y() - half_side - 1e-6)) {
		return false;
	}
	if ((ip.z() > center.z() + half_side + 1e-6) || 
		(ip.z() < center.z() - half_side - 1e-6)) {
		return false;
	}

	double distance = scale_factor;
	if (distance < rec.t) {
		rec.p = ip;
		rec.t = distance;
		rec.normal = normal;
		rec.mat_ptr = mat_ptr;
		return true;
	}
	return false;
	
}

bool cube::hit(ray& ray, double t_min, double t_max, hit_record& rec) const {
	rec.t = infinity;

	hit_side(center.x() - half_side, ray.origin().x(), ray.direction().x(), ray, vec3(-1, 0, 0), rec);
	hit_side(center.x() + half_side, ray.origin().x(), ray.direction().x(), ray, vec3(1, 0, 0), rec);

	hit_side(center.y() - half_side, ray.origin().y(), ray.direction().y(), ray, vec3(0, -1, 0), rec);
	hit_side(center.y() + half_side, ray.origin().y(), ray.direction().y(), ray, vec3(0, 1, 0), rec);

	hit_side(center.z() - half_side, ray.origin().z(), ray.direction().z(), ray, vec3(0, 0, -1), rec);
	hit_side(center.z() + half_side, ray.origin().z(), ray.direction().z(), ray, vec3(0, 0, 1), rec);

	return (rec.t < infinity);
}

#endif // !CUBE_H
