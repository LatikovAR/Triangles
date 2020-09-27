#pragma once

#include <list>

#include "geometry.h"

void make_octahedron(std::list <geometry::triangle> *l_t,
                     const geometry::vec &v1, const geometry::vec &v2, const geometry::vec &v3,
                     const geometry::point &p);
void make_triangle_stack(std::list <geometry::triangle> *l_t,
                         const geometry::triangle &t_base,
                         double step, double range);
