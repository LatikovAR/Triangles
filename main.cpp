#define VISUALIZE

#include <iostream>
#include <list>
#include <cassert>
#include <vector>

#include "geometry.h"
#if !(defined (NORMAL) || defined (VISUALIZE))
#include "triangle_test.h"
#endif
#ifdef VISUALIZE
#include "vulkan_drawing.h"
#endif

geometry::geometry_object input_geometry_object(size_t n);
void print_triangle(const geometry::triangle &t);

#if defined (NORMAL) || defined (VISUALIZE)
geometry::geometry_object input_geometry_object(size_t n) {
    double x, y, z;
    std::cin >> x >> y >> z;
    geometry::point p1(x, y, z);
    std::cin >> x >> y >> z;
    geometry::point p2(x, y, z);
    std::cin >> x >> y >> z;
    geometry::point p3(x, y, z);
    geometry::unknown_geometry_object u_o(p1, p2, p3);
    geometry::g_obj_type o_t = u_o.define_object_type();
    if(o_t == geometry::TRIANGLE) {
        geometry::triangle t = u_o.build_triangle();
        geometry::geometry_object o(o_t, t, n);
        return o;
    }
    if(o_t == geometry::CUT) {
        geometry::cut c = u_o.build_cut();
        geometry::geometry_object o(o_t, c, n);
        return o;
    }
    if(o_t == geometry::POINT) {
        geometry::point p = u_o.build_point();
        geometry::geometry_object o(o_t, p, n);
        return o;
    }
    assert(0);
    geometry::point p(0, 0, 0);
    geometry::geometry_object o(geometry::POINT, p, n);
    return o;
}
#endif

void print_triangle(const geometry::triangle &t) {
    geometry::point p = t.p1_ret();
    std::cout << "p1 " << p.x << " " << p.y << " " << p.z << "\n";
    p = t.p2_ret();
    std::cout << "p2 " << p.x << " " << p.y << " " << p.z << "\n";
    p = t.p3_ret();
    std::cout << "p3 " << p.x << " " << p.y << " " << p.z << "\n";
    std::cout << "\n";
}

int main() {
    size_t n;
    std::list <geometry::geometry_object> l_t;
#if defined (NORMAL) || defined (VISUALIZE)
    std::cin >> n;
#endif
#ifdef TEST0
    test0(&l_t);
#endif
#ifdef TEST1
    test1(&l_t);
#endif
#ifdef TEST2
    test2(&l_t);
#endif
#ifdef TEST3
    test3(&l_t);
#endif
#ifdef TEST4
    test4(&l_t);
#endif
#ifdef TEST5
    test5(&l_t);
#endif
#ifdef TEST6
    test6(&l_t);
#endif
#ifdef TEST7
    test7(&l_t);
#endif
#ifdef TEST8
    test8(&l_t);
#endif
#if defined (NORMAL) || defined (VISUALIZE)
    for(size_t i = 0; i < n; i++) {
        l_t.push_back(input_geometry_object(i));
    }
    assert(l_t.size() == n);
#endif

    n = l_t.size();
    geometry::intersection_finder i_f(n, l_t);
    i_f.find_intersections();
    std::vector <size_t> intersected_o_nums = i_f.intersected_objects_nums();
    for(int i = 0; i < static_cast<int>(intersected_o_nums.size()); i++) {
        std::cout << intersected_o_nums[(size_t) i] << "\n";
    }

#ifdef VISUALIZE
    std::vector <std::pair <geometry::geometry_object, bool>> tr_for_draw = i_f.objs_condition_only_triangles();
    draw_all_triangles(tr_for_draw);
#endif
    return 0;
}
