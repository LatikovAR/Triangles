#pragma once

#include <cassert>
#include <list>
#include <vector>
#include <cmath>

namespace geometry {

enum g_obj_pos {COMMON, PARALLEL, MATCH};
enum g_obj_type {TRIANGLE, CUT, POINT};

struct point {
    double x;
    double y;
    double z;
    point(double a, double b, double c): x(a), y(b), z(c) {}
};

bool is_real_point(const point &p);
bool is_points_match(const point &p1, const point &p2);
bool is_on_one_line(const point &p1, const point &p2, const point &p3);

struct point_2d {
    double x;
    double y;
    point_2d(double a, double b): x(a), y(b) {}
};

struct vec {
    double x;
    double y;
    double z;
    vec(const point &p1, const point &p2) {
        x = p2.x - p1.x;
        y = p2.y - p1.y;
        z = p2.z - p1.z;
    }
    vec(const point_2d &p1, const point_2d &p2) {
        x = p2.x - p1.x;
        y = p2.y - p1.y;
        z = 0;
    }
    vec(double a, double b, double c): x(a), y(b), z(c) {}
};

struct vec_2d {
    double x;
    double y;
    vec_2d(const point_2d &p1, const point_2d &p2) {
        x = p2.x - p1.x;
        y = p2.y - p1.y;
    }
    vec_2d(double a, double b): x(a), y(b) {}
};

vec mult_vec(const vec &v1, const vec &v2);

struct cut {
    point p;
    vec v;
    cut(const point &p1, const point &p2): p(p1), v(p1, p2) {}
    cut(const point &p_in, const vec &v_in): p(p_in), v(v_in) {}
    point p_end() const {
        point p_e(p.x + v.x, p.y + v.y, p.z + v.z);
        return p_e;
    }
};

struct cut_2d {
    point_2d p;
    vec_2d v;
    cut_2d(const point_2d &p1, const point_2d &p2): p(p1), v(p1, p2) {}
    cut_2d(const point_2d &p_in, const vec_2d &v_in): p(p_in), v(v_in) {}
};

g_obj_pos lines_pos_2d(const cut_2d &c1, const cut_2d &c2);
bool is_cut_2d_intersects(const cut_2d &c1, const cut_2d &c2);

struct plane {
    double a;
    double b;
    double c;
    double d;
    plane(const point &p1, const point &p2, const point &p3) {
        vec v1(p1, p2);
        vec v2(p1, p3);
        vec n = mult_vec(v1, v2);
        a = n.x;
        b = n.y;
        c = n.z;
        d = (a * p1.x + b * p1.y + c * p1.z) * (-1);
    }
    double point_side_plane(const point &p) const {
        return (p.x * a + p.y * b + p.z * c + d) / sqrt(a * a + b * b + c * c);
    }
};

g_obj_pos planes_pos(const plane &pl1, const plane &pl2);
g_obj_pos cut_and_plane_pos(const plane &pl, const cut &c);

class triangle {
private:
    point p1;
    point p2;
    point p3;
public:
    triangle(const point &i1, const point &i2, const point &i3): p1(i1), p2(i2), p3(i3) {}
    plane make_plane() const {
        plane p(p1, p2, p3);
        return p;
    }
    point p1_ret() const {
        return p1;
    }
    point p2_ret() const {
        return p2;
    }
    point p3_ret() const {
        return p3;
    }
    bool is_in_triangle(const point &p) const;
};

class triangle_2d {
private:
    point_2d p1;
    point_2d p2;
    point_2d p3;
public:
    triangle_2d(const point_2d &i1, const point_2d &i2, const point_2d &i3): p1(i1), p2(i2), p3(i3) {}
    point_2d p1_ret() const {
        return p1;
    }
    point_2d p2_ret() const {
        return p2;
    }
    point_2d p3_ret() const {
        return p3;
    }
    bool is_in_triangle(const point_2d &p) const;
};

class geometry_object {
private:
    g_obj_type ty;
    union {
        triangle t;
        cut c;
        point p;
    };
    int n;
public:
    geometry_object(g_obj_type character, const triangle &t_in, int number): ty(character), t(t_in), n(number) {}
    geometry_object(g_obj_type character, const cut &c_in, int number): ty(character), c(c_in), n(number) {}
    geometry_object(g_obj_type character, const point &p_in, int number): ty(character), p(p_in), n(number) {}
    int ret_number() const {
        return n;
    }
    g_obj_type ret_type() const {
        return ty;
    }
    triangle ret_triangle() const {
        return t;
    }
    cut ret_cut() const {
        return c;
    }
    point ret_point() const {
        return p;
    }
};

class unknown_geometry_object {
private:
    point p1;
    point p2;
    point p3;
public:
    unknown_geometry_object(const point &i1, const point &i2, const point &i3): p1(i1), p2(i2), p3(i3) {}
    point p1_ret() const {
        return p1;
    }
    point p2_ret() const {
        return p2;
    }
    point p3_ret() const {
        return p3;
    }
    g_obj_type define_object_type() const {
        if((is_points_match(p1, p2)) && (is_points_match(p1, p3))){
            return POINT;
        }
        if((is_points_match(p1, p2)) || (is_points_match(p1, p3)) || (is_points_match(p2, p3))){
            return CUT;
        }
        if(is_on_one_line(p1, p2, p3)) {
            return CUT;
        }
        return TRIANGLE;
    }
    triangle build_triangle() const {
        triangle t(p1, p2, p3);
        return t;
    }
    cut build_cut() const {
        if(is_points_match(p1, p2)) {
            cut c(p1, p3);
            return c;
        }
        if(is_points_match(p1, p3)) {
            cut c(p1, p2);
            return c;
        }
        assert(is_points_match(p2, p3));
        cut c(p1, p2);
        return c;
    }
    point build_point() const {
        return p1;
    }
};

point intersection_pc(const plane &pl, const cut &c);
int geometry_object_side_by_plane(const geometry_object &g, const plane &pl);
bool check_cut_and_triangle_intersection_on_place(const triangle &t, const plane &pl, const cut &c);
bool check_t_and_c_intersection_2d(const triangle_2d &t, const cut_2d &c);
bool check_t_intersection_2d(const triangle_2d &t1, const triangle_2d &t2);
bool check_t_intersection_on_match_pl(const plane &pl, const triangle &t1, const triangle &t2);
bool check_intersection(const triangle &t, const plane &pl, const triangle &t1);
void find_intersected_objects(std::list <geometry_object> &l_t, std::vector <bool> &is_t_intersects);
bool check_intersection(const triangle &t, const plane &pl, const cut &c);
bool check_intersection(const triangle &t, const point &p);
bool check_intersection(const cut &c1, const cut &c2);
bool check_intersection(const cut &c, const point &p);
bool check_intersection(const point &p1, const point &p2);

}
