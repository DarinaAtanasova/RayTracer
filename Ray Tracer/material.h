#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility_functions.h"
#include "texture.h"

struct hit_record;

class material {
	public:
		virtual bool scatter(
			ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		)const = 0;

		virtual color emitted(double u, double v, const point3& p) const {
			return color(0, 0, 0);
		}
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

class dielectric : public material {
	public:
		double index_of_refraction;

	private:
		static double reflectance(double cos, double ref_idx) {
			auto r0 = (1 - ref_idx) / (1 + ref_idx);
			r0 *= r0;
			return r0 + (1 - r0) * pow((1 - cos), 5);
		}

	public:
		dielectric(double ir) : index_of_refraction(ir) {}

		virtual bool scatter(
			ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		)const override {
			attenuation = color(1, 1, 1);
			double refraction_ratio = rec.front_face ? (1.0 / index_of_refraction) : index_of_refraction;
			
			vec3 unit_direction = unit_vector(r_in.direction());
			double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
			double sin_theta = sqrt(1 - (cos_theta * cos_theta));

			bool cannot_refract = refraction_ratio * sin_theta > 1.0;
			vec3 direction;

			if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
				direction = reflect(unit_direction, rec.normal);
			}
			else {
				direction = refract(unit_direction, rec.normal, refraction_ratio);
			}

			scattered = ray(rec.p, direction);
			return true;
		}
};

class diffuse_light : public material {
	public:
		shared_ptr<texture> emit;
		diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

	public:
		diffuse_light(shared_ptr<texture> a) :emit(a) {}
		
		virtual bool scatter(
			ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		)const override {
			return false;
		}

		virtual color emitted(double u, double v, const point3& p) const override {
			return emit->value(u, v, p);
		}
};

#endif // !MATERIAL_H

