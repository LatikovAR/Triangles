//#define NORMAL

#include <iostream>
#include <list>
#include <cassert>
#include <vector>

#include "geometry.h"
#ifndef NORMAL
#include "triangle_test.h"
#endif

geometry::geometry_object input_geometry_object(int n);
void print_triangle(const geometry::triangle &t);

#ifdef TEST0
void test0(std::list <geometry::geometry_object> *l_t) {
    geometry::vec v1(1, 1, 1);
    geometry::vec v2(10, -10, 10);
    geometry::vec v3(-0.01, 1.1, 0.245);
    geometry::point p(0, 0, 0);
    make_octahedron(l_t, v1, v2, v3, p);
}
#endif

#ifdef TEST1
void test1(std::list <geometry::geometry_object> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.01, 1);
}
#endif

#ifdef TEST2
void test2(std::list <geometry::geometry_object> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point(1, 1, 0);
    p2 = geometry::point(1, 2, 0);
    p3 = geometry::point(1, 1, 0.03);
    t = geometry::triangle(p1, p2, p3);
    geometry::geometry_object o(geometry::TRIANGLE, t, (int) l_t.size());
    l_t->push_back(o);
}
#endif

#ifdef TEST3
void test3(std::list <geometry::geometry_object> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point(1, 1, 0);
    p2 = geometry::point(1, 2, 0);
    p3 = geometry::point(1, 1, 0.03);
    t = geometry::triangle(p1, p2, p3);
    geometry::geometry_object o(geometry::TRIANGLE, t, (int) l_t.size());
    l_t->push_back(o);
    p1 = geometry::point(1, 1, 0.5);
    p2 = geometry::point(1, 2, 0.45);
    p3 = geometry::point(1, 1, 0.66);
    t = geometry::triangle(p1, p2, p3);
    geometry::geometry_object o(geometry::TRIANGLE, t, (int) l_t.size());
    l_t->push_back(o);
}
#endif

#ifdef TEST4
void test4(std::list <geometry::geometry_object> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point (0, 0, 0);
    p2 = geometry::point (10, 0, 0);
    p3 = geometry::point (0, 0, 0.5);
    t = geometry::triangle(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.05, -1);
}
#endif

#ifdef TEST5
void test5(std::list <geometry::geometry_object> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point (0, 0, 0);
    p2 = geometry::point (10, 0, 0);
    p3 = geometry::point (0, -10, 0);
    t = geometry::triangle(p1, p2, p3);
    make_triangle_stack(l_t, t, 0.01, -1);
}
#endif

#ifdef NORMAL
geometry::geometry_object input_geometry_object(int n) {
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
    int n;
    std::vector <bool> is_t_intersects;
    std::list <geometry::geometry_object> l_t;
#ifdef NORMAL
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
#ifdef NORMAL
    for(int i = 0; i < n; i++) {
        l_t.push_back(input_geometry_object(i));
    }
    assert((int) l_t.size() == n);
#endif

    n = (int) l_t.size();
    for(int i = 0; i < n; i++) {
        is_t_intersects.push_back(false);
    }
    find_intersected_objects(l_t, is_t_intersects);

    for(int i = 0; i < n; i++) {
        if(is_t_intersects[(size_t) i] == true) {
            std::cout << i << "\n";
        }
    }
    return 0;
}
