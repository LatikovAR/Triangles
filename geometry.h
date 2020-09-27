#pragma once

namespace geometry {

enum g_obj_pos {COMMON, PARALLEL, MATCH};

struct point {
    double x;
    double y;
    double z;
    point(double a, double b, double c): x(a), y(b), z(c) {}
};

bool is_real_point(const point &p);
bool is_points_match(const point &p1, const point &p2);

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
vec vec_on_2p(const point &p1, const point &p2);
vec_2d vec_on_2p_2d(const point_2d &p1, const point_2d &p2);
vec vec_on_2p_2d_to_3d(const point_2d &p1, const point_2d &p2);

struct cut {
    point p;
    vec v;
    cut(const point &p1, const point &p2): p(p1), v(p1, p2) {}
    cut(const point &p_in, const vec &v_in): p(p_in), v(v_in) {}
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
        return p.x * a + p.y * b + p.z * c + d;
    }
};

g_obj_pos planes_pos(const plane &pl1, const plane &pl2);
g_obj_pos cut_and_plane_pos(const plane &pl, const cut &c);

class triangle {
private:
    point p1;
    point p2;
    point p3;
    int number;
public:
    triangle(const point &i1, const point &i2, const point &i3, int n): p1(i1), p2(i2), p3(i3), number(n) {}
    int num() {
        return number;
    }
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

point intersection_pc(const plane &pl, const cut &c);

}
