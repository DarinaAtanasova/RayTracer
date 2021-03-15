cmake_minimum_required (VERSION 3.10)
project (ray_tracer)

set(EMBREE_PATH 
	"C:/Program Files/Intel/Embree3"
	CACHE PATH
	"Where Embree is installed"
)

set(HEADERS
	"Ray Tracer/camera.h"
	"Ray Tracer/color.h"
	"Ray Tracer/cube.h"
	"Ray Tracer/hittable.h"
	"Ray Tracer/hittable_list.h"
	"Ray Tracer/material.h"
	"Ray Tracer/color.h"
	"Ray Tracer/ray.h"
	"Ray Tracer/sphere.h"
	"Ray Tracer/texture.h"
	"Ray Tracer/utility_functions.h"
	"Ray Tracer/vec3.h"
)

set(SOURCES
	"Ray Tracer/main.cpp"
	"Ray Tracer/main.cpp"
	sutil/Mesh.cpp
	sutil/tinyobjloader/tiny_obj_loader.cc
	sutil/rply-1.01/rply.c
)

include_directories(
	${EMBREE_PATH}/include/embree3
	sutil/
)

link_directories(
	${EMBREE_PATH}/lib
)

link_libraries(
	embree3
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

