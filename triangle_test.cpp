#include <list>

#include "triangle_test.h"

void make_octahedron(std::list <triangle> *l_t, vec &v1, vec &v2, vec &v3, point &p) {
    point p1, p2, p3;
    int n;
    p1.x = p.x + v1.x;
    p1.y = p.y + v1.y;
    p1.z = p.z + v1.z;
    p2.x = p.x + v2.x;
    p2.y = p.y + v2.y;
    p2.z = p.z + v2.z;
    p3.x = p.x + v3.x;
    p3.y = p.y + v3.y;
    p3.z = p.z + v3.z;
    n = (int) l_t->size();
    triangle t(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x - v1.x;
    p1.y = p.y - v1.y;
    p1.z = p.z - v1.z;
    p2.x = p.x + v2.x;
    p2.y = p.y + v2.y;
    p2.z = p.z + v2.z;
    p3.x = p.x + v3.x;
    p3.y = p.y + v3.y;
    p3.z = p.z + v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x + v1.x;
    p1.y = p.y + v1.y;
    p1.z = p.z + v1.z;
    p2.x = p.x - v2.x;
    p2.y = p.y - v2.y;
    p2.z = p.z - v2.z;
    p3.x = p.x + v3.x;
    p3.y = p.y + v3.y;
    p3.z = p.z + v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x + v1.x;
    p1.y = p.y + v1.y;
    p1.z = p.z + v1.z;
    p2.x = p.x + v2.x;
    p2.y = p.y + v2.y;
    p2.z = p.z + v2.z;
    p3.x = p.x - v3.x;
    p3.y = p.y - v3.y;
    p3.z = p.z - v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x - v1.x;
    p1.y = p.y - v1.y;
    p1.z = p.z - v1.z;
    p2.x = p.x - v2.x;
    p2.y = p.y - v2.y;
    p2.z = p.z - v2.z;
    p3.x = p.x + v3.x;
    p3.y = p.y + v3.y;
    p3.z = p.z + v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x - v1.x;
    p1.y = p.y - v1.y;
    p1.z = p.z - v1.z;
    p2.x = p.x + v2.x;
    p2.y = p.y + v2.y;
    p2.z = p.z + v2.z;
    p3.x = p.x - v3.x;
    p3.y = p.y - v3.y;
    p3.z = p.z - v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x + v1.x;
    p1.y = p.y + v1.y;
    p1.z = p.z + v1.z;
    p2.x = p.x - v2.x;
    p2.y = p.y - v2.y;
    p2.z = p.z - v2.z;
    p3.x = p.x - v3.x;
    p3.y = p.y - v3.y;
    p3.z = p.z - v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);

    p1.x = p.x - v1.x;
    p1.y = p.y - v1.y;
    p1.z = p.z - v1.z;
    p2.x = p.x - v2.x;
    p2.y = p.y - v2.y;
    p2.z = p.z - v2.z;
    p3.x = p.x - v3.x;
    p3.y = p.y - v3.y;
    p3.z = p.z - v3.z;
    n = (int) l_t->size();
    t.reinit(p1, p2, p3, n);
    l_t->push_back(t);
}
