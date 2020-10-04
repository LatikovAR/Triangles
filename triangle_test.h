#pragma once

#include <list>

#include "geometry.h"

void make_octahedron(std::list <geometry::geometry_object> *l_t,
                     const geometry::vec &v1, const geometry::vec &v2, const geometry::vec &v3,
                     const geometry::point &p);
void make_triangle_stack(std::list <geometry::geometry_object> *l_t,
                         const geometry::triangle &t_base,
                         double step, double range);

#ifdef TEST0
void test0(std::list <geometry_object> *l_t);
#endif

#ifdef TEST1
void test1(std::list <geometry_object> *l_t);
#endif

#ifdef TEST2
void test2(std::list <geometry_object> *l_t);
#endif

#ifdef TEST3
void test3(std::list <geometry_object> *l_t);
#endif

#ifdef TEST4
void test4(std::list <geometry_object> *l_t);
#endif

#ifdef TEST5
void test5(std::list <geometry_object> *l_t);
#endif

#ifdef TEST6
void test6(std::list <geometry_object> *l_t);
#endif
