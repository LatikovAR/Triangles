#include <list>
#include <cassert>
#include <cmath>

#include "triangle_test.h"

using namespace geometry;

void make_octahedron(std::list <geometry_object> *l_t,
                     const vec &v1, const vec &v2, const vec &v3,
                     const point &p) {
    assert(l_t != nullptr);
    int n;
    point p1(p.x + v1.x, p.y + v1.y, p.z + v1.z);
    point p2(p.x + v2.x, p.y + v2.y, p.z + v2.z);
    point p3(p.x + v3.x, p.y + v3.y, p.z + v3.z);
    n = (int) l_t->size();
    triangle t(p1, p2, p3);
    geometry_object o(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x - v1.x, p.y + v1.y, p.z + v1.z);
    p2 = point(p.x - v2.x, p.y + v2.y, p.z + v2.z);
    p3 = point(p.x - v3.x, p.y + v3.y, p.z + v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x + v1.x, p.y - v1.y, p.z + v1.z);
    p2 = point(p.x + v2.x, p.y - v2.y, p.z + v2.z);
    p3 = point(p.x + v3.x, p.y - v3.y, p.z + v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x + v1.x, p.y + v1.y, p.z - v1.z);
    p2 = point(p.x + v2.x, p.y + v2.y, p.z - v2.z);
    p3 = point(p.x + v3.x, p.y + v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x - v1.x, p.y - v1.y, p.z + v1.z);
    p2 = point(p.x - v2.x, p.y - v2.y, p.z + v2.z);
    p3 = point(p.x - v3.x, p.y - v3.y, p.z + v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x - v1.x, p.y + v1.y, p.z - v1.z);
    p2 = point(p.x - v2.x, p.y + v2.y, p.z - v2.z);
    p3 = point(p.x - v3.x, p.y + v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x + v1.x, p.y - v1.y, p.z - v1.z);
    p2 = point(p.x + v2.x, p.y - v2.y, p.z - v2.z);
    p3 = point(p.x + v3.x, p.y - v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);

    p1 = point(p.x - v1.x, p.y - v1.y, p.z - v1.z);
    p2 = point(p.x - v2.x, p.y - v2.y, p.z - v2.z);
    p3 = point(p.x - v3.x, p.y - v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3);
    o = geometry_object(TRIANGLE, t, n);
    l_t->push_back(o);
}

void make_triangle_stack(std::list <geometry::geometry_object> *l_t,
                         const triangle &t_base,
                         double step, double range) {
    assert(l_t != nullptr);
    assert(step > 0);
    triangle t(t_base.p1_ret(), t_base.p2_ret(), t_base.p3_ret());
    geometry_object o(TRIANGLE, t, (int) l_t->size());
    l_t->push_back(o);
    plane pl = t.make_plane();
    vec n(pl.a, pl.b, pl.c);
    if(pl.a < 0) {
        n = vec(-pl.a, -pl.b, -pl.c);
    }
    if(range < 0) {
        range *= -1;
        n.x *= -1;
        n.y *= -1;
        n.z *= -1;
    }
    double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    assert(norm > 0);
    n.x /= norm;
    n.y /= norm;
    n.z /= norm;
    n.x *= step;
    n.y *= step;
    n.z *= step;
    while((range = range - step) >= 0) {
        point p1 = t.p1_ret();
        point p2 = t.p2_ret();
        point p3 = t.p3_ret();
        p1.x += n.x;
        p1.y += n.y;
        p1.z += n.z;
        p2.x += n.x;
        p2.y += n.y;
        p2.z += n.z;
        p3.x += n.x;
        p3.y += n.y;
        p3.z += n.z;
        t = triangle(p1, p2, p3);
        o = geometry_object(TRIANGLE, t, (int) l_t->size());
        l_t->push_back(o);
    }
}
