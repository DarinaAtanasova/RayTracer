#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include<memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : hittable {
	public:
		std::vector<shared_ptr<hittable>> objects;

	public:
		hittable_list(){}
		hittable_list(shared_ptr<hittable> object) {
			add(object);
		}

		void clear() {
			objects.clear();
		}
		void add(shared_ptr<hittable> object) {
			objects.push_back(object);
		}

		virtual bool hit(ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(ray& ray, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (auto& object : objects) {
		if (object->hit(ray, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
			//break;
		}
	}

	return hit_anything;
}

#endif // !HITTABLE_LIST_H

