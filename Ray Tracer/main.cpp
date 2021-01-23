#include "utility_functions.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>
#include<fstream>


// Returns the color of the background
color ray_color(ray r, hittable& world, int depth) {
    hit_record rec;

    // If we go over the ray bounce limit (depth), no more light is gathered
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    //Ignoring hits very near 0
    if (world.hit(r, 0.001, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_unit_vector();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_depth = 50;
    const int samples_per_pixel = 100;

    //World
    
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    //Camera
    camera camera;

    // Render
    std::ofstream file("image.ppm", std::ios::out);
    //if(!file){}
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r = camera.get_ray(u, v);
            color pixel_color = ray_color(r, world, max_depth);

            write_color(file, pixel_color, samples_per_pixel);
        }
    }

    file.close();
    std::cerr << "\nDone.\n";
    system("pause");
}