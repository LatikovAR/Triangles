#include <cmath>
#include <cassert>

#include "geometry.h"

namespace  {
    const double DOUBLE_GAP = 0.000001;
}

bool is_real_point(point &p) {
    if((std::isinf(p.x)) || (std::isnan(p.x))) {
        return false;
    }
    if((std::isinf(p.y)) || (std::isnan(p.y))) {
        return false;
    }
    if((std::isinf(p.z)) || (std::isnan(p.z))) {
        return false;
    }
    return true;
}

bool is_points_match(point &p1, point &p2) {
    if((fabs(p1.x - p2.x) > DOUBLE_GAP) ||
       (fabs(p1.y - p2.y) > DOUBLE_GAP) ||
       (fabs(p1.z - p2.z) > DOUBLE_GAP)) {
        return false;
    }
    return true;
}

vec vec_on_2p(point &p1, point &p2) {
    vec v;
    v.x = p2.x - p1.x;
    v.y = p2.y - p1.y;
    v.z = p2.z - p1.z;
    return v;
}

vec_2d vec_on_2p_2d(point_2d &p1, point_2d &p2) {
    vec_2d v;
    v.x = p2.x - p1.x;
    v.y = p2.y - p1.y;
    return v;
}

vec vec_on_2p_2d_to_3d(point_2d &p1, point_2d &p2) {
    vec v;
    v.x = p2.x - p1.x;
    v.y = p2.y - p1.y;
    v.z = 0;
    return v;
}

vec mult_vec(vec &v1, vec &v2) {
    vec v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

point intersection_pc(plane &pl, cut &c) {
    double k, t;
    point p;
    //pl and c not parallel certainly before this function
    k = pl.a * c.v.x + pl.b * c.v.y + pl.c * c.v.z;
    t = pl.a * c.p.x + pl.b * c.p.y + pl.c * c.p.z + pl.d;
    t = (-1) * t / k;
    p.x = c.p.x + t * c.v.x;
    p.y = c.p.y + t * c.v.y;
    p.z = c.p.z + t * c.v.z;
    return p;
}

bool triangle::is_in_triangle(point &p) {
    if(is_real_point(p) == false) {
        return false;
    }
    vec v1, v2, a1, a2, a3;
    v1 = vec_on_2p(p1, p2);
    v2 = vec_on_2p(p1, p);
    a1 = mult_vec(v1, v2);
    v1 = vec_on_2p(p2, p3);
    v2 = vec_on_2p(p2, p);
    a2 = mult_vec(v1, v2);
    v1 = vec_on_2p(p3, p1);
    v2 = vec_on_2p(p3, p);
    a3 = mult_vec(v1, v2);
    bool ind = true;
    if(a1.x * a2.x < 0) {
        ind = false;
    }
    if(a1.y * a2.y < 0) {
        ind = false;
    }
    if(a1.z * a2.z < 0) {
        ind = false;
    }
    if(a2.x * a3.x < 0) {
        ind = false;
    }
    if(a2.y * a3.y < 0) {
        ind = false;
    }
    if(a2.z * a3.z < 0) {
        ind = false;
    }
    return ind;
}

bool triangle_2d::is_in_triangle(point_2d &p) {
    vec v1, v2, a1, a2, a3;
    v1 = vec_on_2p_2d_to_3d(p1, p2);
    v2 = vec_on_2p_2d_to_3d(p1, p);
    a1 = mult_vec(v1, v2);
    v1 = vec_on_2p_2d_to_3d(p2, p3);
    v2 = vec_on_2p_2d_to_3d(p2, p);
    a2 = mult_vec(v1, v2);
    v1 = vec_on_2p_2d_to_3d(p3, p1);
    v2 = vec_on_2p_2d_to_3d(p3, p);
    a3 = mult_vec(v1, v2);
    bool ind = true;
    if(a1.z * a2.z < 0) {
        ind = false;
    }
    if(a2.z * a3.z < 0) {
        ind = false;
    }
    return ind;
}

g_obj_pos planes_pos(plane &pl1, plane &pl2) {
    double n1, n2;
    n1 = sqrt(pl1.a * pl1.a + pl1.b * pl1.b + pl1.c * pl1.c);
    n2 = sqrt(pl2.a * pl2.a + pl2.b * pl2.b + pl2.c * pl2.c);
    assert(n1 > 0);
    assert(n2 > 0);
    if((fabs(pl1.a / n1 - pl2.a / n2) > DOUBLE_GAP) ||
       (fabs(pl1.b / n1 - pl2.b / n2) > DOUBLE_GAP) ||
       (fabs(pl1.c / n1 - pl2.c / n2) > DOUBLE_GAP)) {
        return COMMON;
    }
    if(abs(pl1.d / n1 - pl2.d / n2) > DOUBLE_GAP) {
        return PARALLEL;
    }
    return MATCH;
}

g_obj_pos cut_and_plane_pos(plane &pl, cut &c) {
    double n;
    n = sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
    assert(n > 0);
    if(fabs(c.v.x * pl.a + c.v.y * pl.b + c.v.z * pl.c) / n > DOUBLE_GAP) {
        return COMMON;
    }
    if(fabs(c.p.x * pl.a + c.p.y * pl.b + c.p.z * pl.c + pl.d) / n > DOUBLE_GAP) {
        return PARALLEL;
    }
    return MATCH;
}

g_obj_pos lines_pos_2d(cut_2d &c1, cut_2d &c2) {
    double n1, n2;
    n1 = sqrt(c1.v.x * c1.v.x + c1.v.y * c1.v.y);
    n2 = sqrt(c2.v.x * c2.v.x + c2.v.y * c2.v.y);
    assert(n1 > 0);
    assert(n2 > 0);
    if((fabs(c1.v.x / n1 - c2.v.x / n2) > DOUBLE_GAP) ||
       (fabs(c1.v.y / n1 - c2.v.y / n2) > DOUBLE_GAP)) {
        return COMMON;
    }
    vec_2d v;
    v.x = c2.p.x - c1.p.x;
    v.y = c2.p.y - c1.p.y;
    double vn;
    vn = sqrt(v.x * v.x + v.y * v.y);
    if(vn > 0) {
        if((fabs(c1.v.x / n1 - v.x / vn) > DOUBLE_GAP) ||
           (fabs(c1.v.y / n1 - v.y / vn) > DOUBLE_GAP)) {
            return PARALLEL;
        }
        else {
            return MATCH;
        }
    }
    else {
        return MATCH;
    }
}

bool is_cut_2d_intersects(cut_2d &c1, cut_2d &c2) {
    g_obj_pos ind;
    ind = lines_pos_2d(c1, c2);
    if(ind == PARALLEL) {
        return false;
    }
    if(ind == MATCH) {
        double a1, a2;
        a1 = (c2.p.x - c1.p.x) / c1.v.x;
        a2 = (c2.p.x + c2.v.x - c1.p.x) / c1.v.x;
        if((std::isinf(a1)) || (std::isnan(a1)) ||
           (std::isinf(a2)) || (std::isnan(a2))) { //because lines can be parallel with coordinate lines
            a1 = (c2.p.y - c1.p.y) / c1.v.y;
            a2 = (c2.p.y + c2.v.y - c1.p.y) / c1.v.y;
        }
        assert(!((std::isinf(a1)) || (std::isnan(a1)) ||
               (std::isinf(a2)) || (std::isnan(a2))));
        if((a1 >= 0) && (a1 <= 1)) {
            return true;
        }
        if((a2 >= 0) && (a2 <= 1)) {
            return true;
        }
        if(a1 * a2 < 0) {
            return true;
        }
        return false;
    }
    assert(ind == COMMON);
    double a, b;
    a = ((c1.p.x - c2.p.x) * c2.v.y - (c1.p.y - c2.p.y) * c2.v.x) / (c1.v.y * c2.v.x - c1.v.x * c2.v.y);
    b = ((c2.p.x - c1.p.x) * c1.v.y - (c2.p.y - c1.p.y) * c1.v.x) / (c2.v.y * c1.v.x - c2.v.x * c1.v.y);
    if((a >= 0) && (a <= 1) && (b >= 0) && (b <= 1)) {
        return true;
    }
    else {
        return false;
    }
}
