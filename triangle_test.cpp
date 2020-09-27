#include <list>

#include "triangle_test.h"

using namespace geometry;

void make_octahedron(std::list <triangle> *l_t, vec &v1, vec &v2, vec &v3, point &p) {
    int n;
    point p1(p.x + v1.x, p.y + v1.y, p.z + v1.z);
    point p2(p.x + v2.x, p.y + v2.y, p.z + v2.z);
    point p3(p.x + v3.x, p.y + v3.y, p.z + v3.z);
    n = (int) l_t->size();
    triangle t(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x - v1.x, p.y + v1.y, p.z + v1.z);
    p2 = point(p.x - v2.x, p.y + v2.y, p.z + v2.z);
    p3 = point(p.x - v3.x, p.y + v3.y, p.z + v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x + v1.x, p.y - v1.y, p.z + v1.z);
    p2 = point(p.x + v2.x, p.y - v2.y, p.z + v2.z);
    p3 = point(p.x + v3.x, p.y - v3.y, p.z + v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x + v1.x, p.y + v1.y, p.z - v1.z);
    p2 = point(p.x + v2.x, p.y + v2.y, p.z - v2.z);
    p3 = point(p.x + v3.x, p.y + v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x - v1.x, p.y - v1.y, p.z + v1.z);
    p2 = point(p.x - v2.x, p.y - v2.y, p.z + v2.z);
    p3 = point(p.x - v3.x, p.y - v3.y, p.z + v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x - v1.x, p.y + v1.y, p.z - v1.z);
    p2 = point(p.x - v2.x, p.y + v2.y, p.z - v2.z);
    p3 = point(p.x - v3.x, p.y + v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x + v1.x, p.y - v1.y, p.z - v1.z);
    p2 = point(p.x + v2.x, p.y - v2.y, p.z - v2.z);
    p3 = point(p.x + v3.x, p.y - v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);

    p1 = point(p.x - v1.x, p.y - v1.y, p.z - v1.z);
    p2 = point(p.x - v2.x, p.y - v2.y, p.z - v2.z);
    p3 = point(p.x - v3.x, p.y - v3.y, p.z - v3.z);
    n = (int) l_t->size();
    t = triangle(p1, p2, p3, n);
    l_t->push_back(t);
}
