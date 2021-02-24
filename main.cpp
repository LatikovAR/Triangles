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

Rotatable_Object input_rot_object(size_t number) {
    Geometry_Object obj(number, input_geometry_object());
    Cut axis(input_axis());
    double speed;
    std::cin >> speed;
    speed = (speed / 180.0) * M_PI; //conversion to radians

    Rotatable_Object r_obj(std::move(obj), std::move(axis), speed);
    return r_obj;
}

int main() {
    size_t n;
    std::cin >> n;

    std::vector<Rotatable_Object> rot_objs;
    rot_objs.reserve(n);

    for(size_t i = 0; i < n; ++i) {
        rot_objs.push_back(input_rot_object(i));
    }

    std::cout << "Input complete.\n\n";

    vulkan::Draw_Triangles_Manager draw{std::move(rot_objs)};
    draw.run();

    return 0;
}
