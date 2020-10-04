#include <cmath>
#include <cassert>
#include <vector>

#include "geometry.h"

namespace geometry {

namespace  {
    const double DOUBLE_GAP = 0.000001;
}

bool is_real_point(const point &p) {
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

bool is_points_match(const point &p1, const point &p2) {
    if((fabs(p1.x - p2.x) > DOUBLE_GAP) ||
       (fabs(p1.y - p2.y) > DOUBLE_GAP) ||
       (fabs(p1.z - p2.z) > DOUBLE_GAP)) {
        return false;
    }
    return true;
}

bool is_on_one_line(const point &p1, const point &p2, const point &p3) {
    vec v1(p1, p2);
    vec v2(p1, p3);
    double n1 = v1.length();
    double n2 = v2.length();
    assert(n1 > 0);
    assert(n2 > 0);
    v1.x /= n1;
    v1.y /= n1;
    v1.z /= n1;
    v2.x /= n2;
    v2.y /= n2;
    v2.z /= n2;
    if(fabs(fabs(v1.x) - fabs(v2.x)) > DOUBLE_GAP) {
        return false;
    }
    if(fabs(fabs(v1.y) - fabs(v2.y)) > DOUBLE_GAP) {
        return false;
    }
    if(fabs(fabs(v1.z) - fabs(v2.z)) > DOUBLE_GAP) {
        return false;
    }
    return true;
}

vec mult_vec(const vec &v1, const vec &v2) {
    double a, b, c;
    a = v1.y * v2.z - v1.z * v2.y;
    b = v1.z * v2.x - v1.x * v2.z;
    c = v1.x * v2.y - v1.y * v2.x;
    vec v(a, b, c);
    return v;
}

point intersection_pc(const plane &pl, const cut &c) {
    double k, t, x, y, z;
    //pl and c not parallel certainly before this function
    k = pl.a * c.v.x + pl.b * c.v.y + pl.c * c.v.z;
    t = pl.a * c.p.x + pl.b * c.p.y + pl.c * c.p.z + pl.d;
    t = (-1) * t / k;
    x = c.p.x + t * c.v.x;
    y = c.p.y + t * c.v.y;
    z = c.p.z + t * c.v.z;
    point p(x, y, z);
    return p;
}

bool triangle::is_in_triangle(const point &p) const {
    if(is_real_point(p) == false) {
        return false;
    }
    vec v1(p1, p2);
    vec v2(p1, p);
    vec a1 = mult_vec(v1, v2);
    v1 = vec(p2, p3);
    v2 = vec(p2, p);
    vec a2 = mult_vec(v1, v2);
    v1 = vec(p3, p1);
    v2 = vec(p3, p);
    vec a3 = mult_vec(v1, v2);
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

bool triangle_2d::is_in_triangle(const point_2d &p) const {
    vec v1(p1, p2);
    vec v2(p1, p);
    vec a1 = mult_vec(v1, v2);
    v1 = vec(p2, p3);
    v2 = vec(p2, p);
    vec a2 = mult_vec(v1, v2);
    v1 = vec(p3, p1);
    v2 = vec(p3, p);
    vec a3 = mult_vec(v1, v2);
    bool ind = true;
    if(a1.z * a2.z < 0) {
        ind = false;
    }
    if(a2.z * a3.z < 0) {
        ind = false;
    }
    return ind;
}

g_obj_pos planes_pos(const plane &pl1, const plane &pl2) {
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

g_obj_pos cut_and_plane_pos(const plane &pl, const cut &c) {
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

g_obj_pos lines_pos_2d(const cut_2d &c1, const cut_2d &c2) {
    double n1, n2;
    n1 = sqrt(c1.v.x * c1.v.x + c1.v.y * c1.v.y);
    n2 = sqrt(c2.v.x * c2.v.x + c2.v.y * c2.v.y);
    assert(n1 > 0);
    assert(n2 > 0);
    if((fabs(c1.v.x / n1 - c2.v.x / n2) > DOUBLE_GAP) ||
       (fabs(c1.v.y / n1 - c2.v.y / n2) > DOUBLE_GAP)) {
        return COMMON;
    }
    vec_2d v(c2.p.x - c1.p.x, c2.p.y - c1.p.y);
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

bool is_cut_2d_intersects(const cut_2d &c1, const cut_2d &c2) {
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

int geometry_object_side_by_plane(const geometry_object &g, const plane &pl) {
    double i1, i2, i3;
    if(g.ret_type() == TRIANGLE) {
        triangle t = g.ret_triangle();
        point p = t.p1_ret();
        i1 = pl.point_side_plane(p);
        p = t.p2_ret();
        i2 = pl.point_side_plane(p);
        p = t.p3_ret();
        i3 = pl.point_side_plane(p);
        if((i1 > 0) && (i2 > 0) && (i3 > 0)) {
            return 1;
        }
        if((i1 < 0) && (i2 < 0) && (i3 < 0)) {
            return -1;
        }
        return 0;
    }
    if(g.ret_type() == CUT) {
        cut c = g.ret_cut();
        point p = c.p;
        i1 = pl.point_side_plane(p);
        p = c.p_end();
        i2 = pl.point_side_plane(p);
        if((i1 > 0) && (i2 > 0)) {
            return 1;
        }
        if((i1 < 0) && (i2 < 0)) {
            return -1;
        }
        return 0;
    }
    if(g.ret_type() == POINT) {
        point p = g.ret_point();
        i1 = pl.point_side_plane(p);
        if(i1 > 0) {
            return 1;
        }
        if(i1 < 0) {
            return -1;
        }
        return 0;
    }
    assert(0);
    return 0;
}

bool check_cut_and_triangle_intersection_on_place(const triangle &t, const plane &pl, const cut &c) {
    //moving to 2d with checking case if pl is parallel to coordinate plane
    //c lies on pl certainly
    if((abs(pl.a) <= abs(pl.c)) && (abs(pl.b) <= abs(pl.c))) {
        point_2d p1_2d(t.p1_ret().x, t.p1_ret().y);
        point_2d p2_2d(t.p2_ret().x, t.p2_ret().y);
        point_2d p3_2d(t.p3_ret().x, t.p3_ret().y);
        triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        vec_2d v_2d(c.v.x, c.v.y);
        p1_2d = point_2d(c.p.x, c.p.y);
        cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
    else if((abs(pl.a) <= abs(pl.b)) && (abs(pl.c) <= abs(pl.b))) {
        point_2d p1_2d(t.p1_ret().x, t.p1_ret().z);
        point_2d p2_2d(t.p2_ret().x, t.p2_ret().z);
        point_2d p3_2d(t.p3_ret().x, t.p3_ret().z);
        triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        vec_2d v_2d(c.v.x, c.v.z);
        p1_2d = point_2d(c.p.x, c.p.z);
        cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
    else {
        point_2d p1_2d(t.p1_ret().y, t.p1_ret().z);
        point_2d p2_2d(t.p2_ret().y, t.p2_ret().z);
        point_2d p3_2d(t.p3_ret().y, t.p3_ret().z);
        triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        vec_2d v_2d(c.v.y, c.v.z);
        p1_2d = point_2d(c.p.y, c.p.z);
        cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
}

bool check_t_and_c_intersection_2d(const triangle_2d &t, const cut_2d &c) {
    //check points lie into triangle
    point_2d p1 = c.p;
    if(t.is_in_triangle(p1)) {
        return true;
    }
    p1.x = c.p.x + c.v.x;
    p1.y = c.p.y + c.v.y;
    if(t.is_in_triangle(p1)) {
        return true;
    }

    //searching for cuts intersection
    p1 = t.p1_ret();
    point_2d p2 = t.p2_ret();
    cut_2d c_t1(p1, p2);
    if(is_cut_2d_intersects(c, c_t1)) return true;
    p1 = t.p1_ret();
    p2 = t.p3_ret();
    cut_2d c_t2(p1, p2);
    if(is_cut_2d_intersects(c, c_t2)) return true;
    p1 = t.p2_ret();
    p2 = t.p3_ret();
    cut_2d c_t3(p1, p2);
    if(is_cut_2d_intersects(c, c_t3)) return true; //excess
    return false;
}

bool check_t_intersection_2d(const triangle_2d &t1, const triangle_2d &t2) {
    //checking case if triangle corners lie into other triangle
    point_2d p1 = t2.p1_ret();
    if(t1.is_in_triangle(p1)) return true;
    p1 = t2.p2_ret();
    if(t1.is_in_triangle(p1)) return true;
    p1 = t2.p3_ret();
    if(t1.is_in_triangle(p1)) return true;

    p1 = t1.p1_ret();
    if(t2.is_in_triangle(p1)) return true;
    p1 = t1.p2_ret();
    if(t2.is_in_triangle(p1)) return true;
    p1 = t1.p3_ret();
    if(t2.is_in_triangle(p1)) return true;

    //searching for cuts intersection
    p1 = t1.p1_ret();
    point_2d p2 = t1.p2_ret();
    cut_2d c11(p1, p2); // why cut_2d c11(t1.p1_ret(), t1.p2_ret()); not works?
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    cut_2d c12(p1, p2);
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    cut_2d c13(p1, p2);

    p1 = t2.p1_ret();
    p2 = t2.p2_ret();
    cut_2d c21(p1, p2);
    if(is_cut_2d_intersects(c11, c21)) return true;
    if(is_cut_2d_intersects(c12, c21)) return true;
    if(is_cut_2d_intersects(c13, c21)) return true;

    p1 = t2.p1_ret();
    p2 = t2.p3_ret();
    cut_2d c22(p1, p2);
    if(is_cut_2d_intersects(c11, c22)) return true;
    if(is_cut_2d_intersects(c12, c22)) return true;
    if(is_cut_2d_intersects(c13, c22)) return true;

    p1 = t2.p2_ret();
    p2 = t2.p3_ret();
    cut_2d c23(p1, p2);
    if(is_cut_2d_intersects(c11, c23)) return true;
    if(is_cut_2d_intersects(c12, c23)) return true;
    if(is_cut_2d_intersects(c13, c23)) return true; //it is excess

    return false;
}

bool check_common_way_intersection(const triangle &t, const plane &pl, const triangle &t1) {
    std::vector <point> arr_p;
    g_obj_pos c1_ind, c2_ind, c3_ind;

    point p1 = t1.p1_ret();
    point p2 = t1.p2_ret();
    cut c1(p1, p2);
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    cut c2(p1, p2);
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    cut c3(p1, p2);

    c1_ind = cut_and_plane_pos(pl, c1);
    c2_ind = cut_and_plane_pos(pl, c2);
    c3_ind = cut_and_plane_pos(pl, c3);
    //cases if some t1 cut lies on pl
    if(c1_ind == MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c1);
    }
    if(c2_ind == MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c2);
    }
    if(c3_ind == MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c3);
    }

    //common way: searching two points which t1 cuts intersect pl
    p1 = t1.p1_ret();
    p2 = t1.p2_ret();
    if((c1_ind == COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        arr_p.push_back(intersection_pc(pl, c1));
    }
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    if((c2_ind == COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        arr_p.push_back(intersection_pc(pl, c2));
    }
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    if((arr_p.size() < 2) && (c3_ind == COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        arr_p.push_back(intersection_pc(pl, c3));
    }
    assert(arr_p.size() == 2);

    //case if triangle corner lies on pl
    if(is_points_match(arr_p[0], arr_p[1])) {
        return t.is_in_triangle(arr_p[0]);
    }

    cut c(arr_p[0], arr_p[1]);
    return check_cut_and_triangle_intersection_on_place(t, pl, c);
}

bool check_t_intersection_on_match_pl(const plane &pl, const triangle &t1, const triangle &t2) {
    //moving to 2d with checking case if pl is parallel to coordinate plane
    if((abs(pl.a) <= abs(pl.c)) && (abs(pl.b) <= abs(pl.c))) {
        point_2d p1_2d(t1.p1_ret().x, t1.p1_ret().y);
        point_2d p2_2d(t1.p2_ret().x, t1.p2_ret().y);
        point_2d p3_2d(t1.p3_ret().x, t1.p3_ret().y);
        triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d = point_2d(t2.p1_ret().x, t2.p1_ret().y);
        p2_2d = point_2d(t2.p2_ret().x, t2.p2_ret().y);
        p3_2d = point_2d(t2.p3_ret().x, t2.p3_ret().y);
        triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
    else if((abs(pl.a) <= abs(pl.b)) && (abs(pl.c) <= abs(pl.b))) {
        point_2d p1_2d(t1.p1_ret().x, t1.p1_ret().z);
        point_2d p2_2d(t1.p2_ret().x, t1.p2_ret().z);
        point_2d p3_2d(t1.p3_ret().x, t1.p3_ret().z);
        triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d = point_2d(t2.p1_ret().x, t2.p1_ret().z);
        p2_2d = point_2d(t2.p2_ret().x, t2.p2_ret().z);
        p3_2d = point_2d(t2.p3_ret().x, t2.p3_ret().z);
        triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
    else {
        point_2d p1_2d(t1.p1_ret().y, t1.p1_ret().z);
        point_2d p2_2d(t1.p2_ret().y, t1.p2_ret().z);
        point_2d p3_2d(t1.p3_ret().y, t1.p3_ret().z);
        triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d = point_2d(t2.p1_ret().y, t2.p1_ret().z);
        p2_2d = point_2d(t2.p2_ret().y, t2.p2_ret().z);
        p3_2d = point_2d(t2.p3_ret().y, t2.p3_ret().z);
        triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
}

bool check_intersection(const triangle &t, const plane &pl, const triangle &t1) {
    plane pl1 = t1.make_plane();
    g_obj_pos p_pos = planes_pos(pl, pl1);
    if(p_pos == PARALLEL) {
        return false;
    }
    if(p_pos == MATCH) {
        return check_t_intersection_on_match_pl(pl, t, t1);
    }
    if(p_pos == COMMON) {
        return check_common_way_intersection(t, pl, t1);
    }
    assert(0);
    return false;
}

bool check_intersection(const triangle &t, const plane &pl, const cut &c) {
    point p = intersection_pc(pl, c);
    return t.is_in_triangle(p);
}

bool check_intersection(const triangle &t, const point &p) {
    return t.is_in_triangle(p);
}

bool check_intersection(const cut &c1, const cut &c2) {
    plane pl(c1.p, c1.p_end(), c2.p);
    double i = pl.point_side_plane(c2.p_end());
    if(fabs(i) > DOUBLE_GAP) {
        return false;
    }
    if((fabs(pl.a) >= fabs(pl.b)) && (fabs(pl.a) >= fabs(pl.c))) {
        point_2d p_2d(c1.p.y, c1.p.z);
        vec_2d v_2d(c1.v.y, c1.v.z);
        cut_2d c1_2d(p_2d, v_2d);
        p_2d = point_2d(c2.p.y, c2.p.z);
        v_2d = vec_2d(c2.v.y, c2.v.z);
        cut_2d c2_2d(p_2d, v_2d);
        return is_cut_2d_intersects(c1_2d, c2_2d);
    }
    if((fabs(pl.b) >= fabs(pl.a)) && (fabs(pl.b) >= fabs(pl.c))) {
        point_2d p_2d(c1.p.x, c1.p.z);
        vec_2d v_2d(c1.v.x, c1.v.z);
        cut_2d c1_2d(p_2d, v_2d);
        p_2d = point_2d(c2.p.x, c2.p.z);
        v_2d = vec_2d(c2.v.x, c2.v.z);
        cut_2d c2_2d(p_2d, v_2d);
        return is_cut_2d_intersects(c1_2d, c2_2d);
    }
    point_2d p_2d(c1.p.x, c1.p.y);
    vec_2d v_2d(c1.v.x, c1.v.y);
    cut_2d c1_2d(p_2d, v_2d);
    p_2d = point_2d(c2.p.x, c2.p.y);
    v_2d = vec_2d(c2.v.x, c2.v.y);
    cut_2d c2_2d(p_2d, v_2d);
    return is_cut_2d_intersects(c1_2d, c2_2d);
}

bool check_intersection(const cut &c, const point &p) {
    if((fabs(c.v.x) >= fabs(c.v.y)) && (fabs(c.v.x) >= fabs(c.v.z))) {
        assert(fabs(c.v.x) > DOUBLE_GAP);
        double k = (p.x - c.p.x) / c.v.x;
        if((k < 0) || (k > 1)) {
            return false;
        }
        if(fabs(c.p.y + c.v.y * k - p.y) > DOUBLE_GAP) {
            return false;
        }
        if(fabs(c.p.z + c.v.z * k - p.z) > DOUBLE_GAP) {
            return false;
        }
        return true;
    }
    if((fabs(c.v.y) >= fabs(c.v.x)) && (fabs(c.v.y) >= fabs(c.v.z))) {
        assert(fabs(c.v.y) > DOUBLE_GAP);
        double k = (p.y - c.p.y) / c.v.y;
        if((k < 0) || (k > 1)) {
            return false;
        }
        if(fabs(c.p.x + c.v.x * k - p.x) > DOUBLE_GAP) {
            return false;
        }
        if(fabs(c.p.z + c.v.z * k - p.z) > DOUBLE_GAP) {
            return false;
        }
        return true;
    }
    assert(fabs(c.v.z) > DOUBLE_GAP);
    double k = (p.z - c.p.z) / c.v.z;
    if((k < 0) || (k > 1)) {
        return false;
    }
    if(fabs(c.p.x + c.v.x * k - p.x) > DOUBLE_GAP) {
        return false;
    }
    if(fabs(c.p.y + c.v.y * k - p.y) > DOUBLE_GAP) {
        return false;
    }
    return true;
}

bool check_intersection(const point &p1, const point &p2) {
    return is_points_match(p1, p2);
}

void find_intersected_objects(std::list <geometry_object> &l_t, std::vector <bool> &is_t_intersects) {
    assert(l_t.size() > 0);
    geometry_object root_object = l_t.front();
    l_t.pop_front();
    if(l_t.size() > 0) {
        if(root_object.ret_type() == TRIANGLE) {
            triangle t_root = root_object.ret_triangle();
            plane pl = t_root.make_plane();
            std::list <geometry_object> l_t_new;
            std::list <geometry_object> l_t1;
            std::list <geometry_object> l_t2;
            while(l_t.size() > 0) {
                geometry_object o = l_t.front();
                l_t.pop_front();
                l_t_new.push_back(o);
                int i = geometry_object_side_by_plane(o, pl);
                if(i == -1) {
                    l_t1.push_back(o);
                }
                if(i == 1) {
                    l_t2.push_back(o);
                }
                if(i == 0) {
                    l_t1.push_back(o);
                    l_t2.push_back(o);
                    if((is_t_intersects[(size_t) root_object.ret_number()] == false) ||
                       (is_t_intersects[(size_t) o.ret_number()] == false)) {
                        if(o.ret_type() == TRIANGLE) {
                            triangle t = o.ret_triangle();
                            if(check_intersection(t_root, pl, t)) {
                                is_t_intersects[(size_t) root_object.ret_number()] = true;
                                is_t_intersects[(size_t) o.ret_number()] = true;
                            }
                        }
                        if(o.ret_type() == CUT) {
                            cut c = o.ret_cut();
                            if(check_intersection(t_root, pl, c)) {
                                is_t_intersects[(size_t) root_object.ret_number()] = true;
                                is_t_intersects[(size_t) o.ret_number()] = true;
                            }
                        }
                        if(o.ret_type() == POINT) {
                            point p = o.ret_point();
                            if(check_intersection(t_root, p)) {
                                is_t_intersects[(size_t) root_object.ret_number()] = true;
                                is_t_intersects[(size_t) o.ret_number()] = true;
                            }
                        }
                    }
                }
            }
            if((l_t1.size() < l_t_new.size()) && (l_t2.size() < l_t_new.size())) {
                l_t_new.clear();
                find_intersected_objects(l_t1, is_t_intersects);
                find_intersected_objects(l_t2, is_t_intersects);
            }
            else {
                l_t1.clear();
                l_t2.clear();
                find_intersected_objects(l_t_new, is_t_intersects);
            }
        }
        if(root_object.ret_type() == CUT) {
            cut c_root = root_object.ret_cut();
            std::list <geometry_object> l_t_new;
            while(l_t.size() > 0) {
                geometry_object o = l_t.front();
                l_t.pop_front();
                l_t_new.push_back(o);
                if((is_t_intersects[(size_t) root_object.ret_number()] == false) ||
                   (is_t_intersects[(size_t) o.ret_number()] == false)) {
                    if(o.ret_type() == TRIANGLE) {
                        triangle t = o.ret_triangle();
                        plane pl = t.make_plane();
                        if(check_intersection(t, pl, c_root)) {
                            is_t_intersects[(size_t) root_object.ret_number()] = true;
                            is_t_intersects[(size_t) o.ret_number()] = true;
                        }
                    }
                    if(o.ret_type() == CUT) {
                        cut c = o.ret_cut();
                        if(check_intersection(c_root, c)) {
                            is_t_intersects[(size_t) root_object.ret_number()] = true;
                            is_t_intersects[(size_t) o.ret_number()] = true;
                        }
                    }
                    if(o.ret_type() == POINT) {
                        point p = o.ret_point();
                        if(check_intersection(c_root, p)) {
                            is_t_intersects[(size_t) root_object.ret_number()] = true;
                            is_t_intersects[(size_t) o.ret_number()] = true;
                        }
                    }
                }
            }
            find_intersected_objects(l_t_new, is_t_intersects);
        }
        if(root_object.ret_type() == POINT) {
            point p_root = root_object.ret_point();
            std::list <geometry_object> l_t_new;
            while(l_t.size() > 0) {
                geometry_object o = l_t.front();
                l_t.pop_front();
                l_t_new.push_back(o);
                if((is_t_intersects[(size_t) root_object.ret_number()] == false) ||
                   (is_t_intersects[(size_t) o.ret_number()] == false)) {
                    if(o.ret_type() == TRIANGLE) {
                        triangle t = o.ret_triangle();
                        if(check_intersection(t, p_root)) {
                            is_t_intersects[(size_t) root_object.ret_number()] = true;
                            is_t_intersects[(size_t) o.ret_number()] = true;
                        }
                    }
                    if(o.ret_type() == CUT) {
                        cut c = o.ret_cut();
                        if(check_intersection(c, p_root)) {
                            is_t_intersects[(size_t) root_object.ret_number()] = true;
                            is_t_intersects[(size_t) o.ret_number()] = true;
                        }
                    }
                    if(o.ret_type() == POINT) {
                        point p = o.ret_point();
                        if(check_intersection(p_root, p)) {
                            is_t_intersects[(size_t) root_object.ret_number()] = true;
                            is_t_intersects[(size_t) o.ret_number()] = true;
                        }
                    }
                }
            }
            find_intersected_objects(l_t_new, is_t_intersects);
        }
    }
}
}
