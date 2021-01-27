#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility_functions.h"

struct hit_record;

class material {
	public:
		virtual bool scatter(
			ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		)const = 0;
};

class lambertian : public material {
	public:
		color albedo; //the reflecting power of a material
	public:
		lambertian(const color& a) : albedo(a) {}

		virtual bool scatter(
			ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		)const override {
			auto scatter_direction = rec.normal + random_unit_vector();

			if (scatter_direction.near_zero()) {
				scatter_direction = rec.normal;
			}

			scattered = ray(rec.p, scatter_direction);
			attenuation = albedo;
			return true;
		}
};

class metal : public material {
	public:
		color albedo;
		double fuzzines;

	public:
		metal(const color& a, double f) : albedo(a), fuzzines(f < 1 ? f : 1) {}

		virtual bool scatter(
			ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		)const override {
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = ray(rec.p, reflected + fuzzines*random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}
};

#endif // !MATERIAL_H

