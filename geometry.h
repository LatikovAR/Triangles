#pragma once

enum g_obj_pos {COMMON, PARALLEL, MATCH};

struct point {
    double x;
    double y;
    double z;
};

struct point_2d {
    double x;
    double y;
};

struct vec {
    double x;
    double y;
    double z;
};

struct vec_2d {
    double x;
    double y;
};

vec mult_vec(vec &v1, vec &v2);
vec vec_on_2p(point &p1, point &p2);
vec_2d vec_on_2p_2d(point_2d &p1, point_2d &p2);
vec vec_on_2p_2d_to_3d(point_2d &p1, point_2d &p2);

struct cut {
    point p;
    vec v;
    cut(point &p1, point &p2)
    {
        p = p1;
        v = vec_on_2p(p1, p2);
    }
    cut(point &p_in, vec &v_in)
    {
        p = p_in;
        v = v_in;
    }
};

struct cut_2d {
    point_2d p;
    vec_2d v;
    cut_2d(point_2d &p1, point_2d &p2)
    {
        p = p1;
        v = vec_on_2p_2d(p1, p2);
    }
    cut_2d(point_2d &p_in, vec_2d &v_in)
    {
        p = p_in;
        v = v_in;
    }
};

g_obj_pos lines_pos_2d(cut_2d &c1, cut_2d &c2);
bool is_cut_2d_intersects(cut_2d &c1, cut_2d &c2);

struct plane {
    double a;
    double b;
    double c;
    double d;
    plane(point &p1, point &p2, point &p3) {
        vec v1 = vec_on_2p(p1, p2);
        vec v2 = vec_on_2p(p1, p3);
        vec n = mult_vec(v1, v2);
        a = n.x;
        b = n.y;
        c = n.z;
        d = (a * p1.x + b * p1.y + c * p1.z) * (-1);
    }
    double point_side_plane(point &p) {
        return p.x * a + p.y * b + p.z * c + d;
    }
};

g_obj_pos planes_pos(plane &pl1, plane &pl2);
g_obj_pos cut_and_plane_pos(plane &pl, cut &c);

class triangle {
private:
    point p1;
    point p2;
    point p3;
    int number;
public:
    triangle(point &i1, point &i2, point &i3, int n) {
        p1 = i1;
        p2 = i2;
        p3 = i3;
        number = n;
    }
    int num() {
        return number;
    }
    plane make_plane() {
        plane p(p1, p2, p3);
        return p;
    }
    point p1_ret() {
        return p1;
    }
    point p2_ret() {
        return p2;
    }
    point p3_ret() {
        return p3;
    }
    bool is_in_triangle(point &p);
};

class triangle_2d {
private:
    point_2d p1;
    point_2d p2;
    point_2d p3;
public:
    triangle_2d(point_2d &i1, point_2d &i2, point_2d &i3) {
        p1 = i1;
        p2 = i2;
        p3 = i3;
    }
    point_2d p1_ret() {
        return p1;
    }
    point_2d p2_ret() {
        return p2;
    }
    point_2d p3_ret() {
        return p3;
    }
    bool is_in_triangle(point_2d &p);
};

point intersection_pc(plane &pl, cut &c);
