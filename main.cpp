#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <list>
#include <cassert>
#include <vector>

#include "geometry.h"
#include "intersection_finder.h"
#include "rotator.h"
#include "vulkan_drawing.h"

using namespace geometry;

Undefined_Object input_geometry_object() {
    double x, y, z;
    std::cin >> x >> y >> z;
    point p1(x, y, z);
    std::cin >> x >> y >> z;
    point p2(x, y, z);
    std::cin >> x >> y >> z;
    point p3(x, y, z);
    return Undefined_Object(p1, p2, p3);
}

Cut input_axis() {
    double x, y, z;
    std::cin >> x >> y >> z;
    point p1(x, y, z);
    std::cin >> x >> y >> z;
    point p2(x, y, z);
    return Cut(p1, p2);
}

int main() {
    size_t n;
    std::cin >> n;

    std::vector<Geometry_Object> objects;
    std::vector<Cut> axes;
    std::vector<double> speed;
    objects.reserve(n);
    axes.reserve(n);
    speed.reserve(n);

    for(size_t i = 0; i < n; i++) {
        objects.push_back(Geometry_Object(i, input_geometry_object()));
        axes.push_back(input_axis());
        double sp;
        std::cin >> sp;
        sp = (sp / 180.0) * M_PI; //conversion to radians
        speed.push_back(sp);
    }

    std::cout << "Input complete.\n";

    Rotator rotate(std::move(objects), std::move(axes), std::move(speed));
    for(int i = 0; i < 19; ++i) {
        std::vector<Geometry_Object> rot_objects = rotate.cur_objects_pos(i * 1000);
        std::cout << "sec: " << i << std::endl;
        for(const auto& obj : rot_objects) {
            obj.print();
            std::cout << std::endl;
        }
    }

    /*
    Intersection_Finder intersection_finder{std::move(objects)};
    Objects_and_Intersections intersection_defined_objects = intersection_finder.compute_intersections();
    const std::vector<bool>& intersection_flags = intersection_defined_objects.intersection_flags();

    std::cout << "Intersected objects:" << std::endl;
    for(size_t i = 0; i < intersection_flags.size(); ++i) {
        if(intersection_flags[i] == true) {
            std::cout << i << std::endl;
        }
    }
    std::cout << std::endl;
    draw_triangles_driver(std::move(intersection_defined_objects)); */
    return 0;
}
