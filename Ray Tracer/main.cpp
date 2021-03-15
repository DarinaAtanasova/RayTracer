#include "utility_functions.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "cube.h"

//Embree
#include "rtcore.h"

#include <iostream>
#include <fstream>

#include "Mesh.h"


auto material_bunny = make_shared<lambertian>(color(0.5, 0.3, 0.0));


// Returns the color of the background
color ray_color(ray &R, hittable& world, int depth, RTCScene &scene, Mesh &mesh) {
    hit_record rec;

    // If we go over the ray bounce limit (depth), no more light is gathered
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    RTCRayHit rh;
    RTCRay& r = rh.ray;
    r.org_x = R.orig.x();
    r.org_y = R.orig.y();
    r.org_z = R.orig.z();
    r.tnear = 0;
    r.dir_x = R.dir.x();
    r.dir_y = R.dir.y();
    r.dir_z = R.dir.z();
    r.tfar = std::numeric_limits<float>::infinity();
    r.mask = -1;
    r.flags = 0;

    rh.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rh.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    // Perform ray intersection
    rtcIntersect1(scene, &context, &rh);
    if (rh.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        Triangle* triangles = (Triangle*)mesh.getVertexIndices();
        Triangle t = triangles[rh.hit.primID];
        Vertex* vertices = (Vertex*)mesh.getVertexData();
        point3 v0 = make_point(vertices[t.v0]);
        point3 v1 = make_point(vertices[t.v1]);
        point3 v2 = make_point(vertices[t.v2]);
        vec3 ab = v1 - v0;
        vec3 ac = v2 - v0;
        rec.p = v0 + (ab * rh.hit.u) + (ac * rh.hit.v);
        rec.normal = vec3(rh.hit.Ng_x, rh.hit.Ng_y, rh.hit.Ng_z);
        rec.mat_ptr = material_bunny;
        rec.t = (rec.p - R.origin()).length();

        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(R, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1, scene, mesh);
        }
        return color(0, 1, 0);
    }
    

    //Ignoring hits very near 0
    if (world.hit(R, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(R, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1, scene, mesh);
        }
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(R.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}



int main() {
    // Open obj file (3D model)
    Mesh bunny_mesh;
    loadMesh("./3D objects/bunny.obj", bunny_mesh);
    int triangles = bunny_mesh.num_triangles;
    auto vertex_data = bunny_mesh.getVertexData();

    //  Embree

     // Inititiate device and scene
    RTCDevice device = rtcNewDevice("");
    RTCScene scene = rtcNewScene(device);

    RTCGeometry mesh = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    Vertex* vertices = (Vertex*)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(Vertex), bunny_mesh.num_vertices);
    memcpy(vertices, vertex_data, sizeof(Vertex) * bunny_mesh.num_vertices);
        
    int* indices = (int*)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(int) * 3, bunny_mesh.num_triangles);
    int * vertex_indices = bunny_mesh.getVertexIndices();
    memcpy(indices, vertex_indices, sizeof(int) * 3 * bunny_mesh.num_triangles);

    // Commit geometry to the scene
    rtcCommitGeometry(mesh);
    unsigned int geomID = rtcAttachGeometry(scene, mesh);
    rtcReleaseGeometry(mesh);
    rtcCommitScene(scene);

    
    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_depth = 50;
    const int samples_per_pixel = 100;

    //World
    
    auto R = cos(pi / 4);
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.5, 0.1, 1));
    auto material_left = make_shared<metal>(color(0.5, 0.6, 0.6), 0.2);
    auto material_right = make_shared<dielectric>(1.5);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(-2.0, 0.0, -1.0), 0.5, material_right));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_left));

    //Camera
    camera camera(point3(-1, 0.5, 5), point3(-1, 0, 0), vec3(0, -1, 0), 20, aspect_ratio); // front camera

    // Render
    std::ofstream file("image.ppm", std::ios::out);
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray R = camera.get_ray(u, v);
                R.dir = unit_vector(R.dir);
                pixel_color += ray_color(R, world, max_depth, scene, bunny_mesh);
            }
            write_color(file, pixel_color, samples_per_pixel);
        }
    }

    rtcReleaseScene(scene);
    rtcReleaseDevice(device);

    file.close();
    std::cerr << "\nDone.\n";
    system("pause");
}
