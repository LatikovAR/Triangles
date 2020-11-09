#pragma once

#include <cassert>
#include <list>
#include <vector>
#include <cmath>
#include <utility>

namespace geometry {

enum g_obj_pos {COMMON, PARALLEL, MATCH};
enum g_obj_type {TRIANGLE, CUT, POINT};

struct vec;
struct vec_2d;

struct point final {
    double x;
    double y;
    double z;
    point(double a, double b, double c): x(a), y(b), z(c) {}
    point& operator+=(const vec& v);
};

bool is_real_point(const point &p);
bool is_points_match(const point &p1, const point &p2);
bool is_on_one_line(const point &p1, const point &p2, const point &p3);

struct point_2d final {
    double x;
    double y;
    point_2d(double a, double b): x(a), y(b) {}
    point_2d& operator+=(const vec_2d& v);
};

struct vec final {
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

    double length() const {
        return sqrt(x * x + y * y + z * z);
    }
    vec& operator+=(const vec& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    vec& operator*=(double a) {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }
    vec& operator/=(double a) {
        a = 1 / a;
        return (*this *= a);
    }
};

point operator+(const point& p, const vec& v);
point operator+(const vec& v, const point& p);

struct vec_2d final {
    double x;
    double y;
    vec_2d(const point_2d &p1, const point_2d &p2) {
        x = p2.x - p1.x;
        y = p2.y - p1.y;
    }
    vec_2d(double a, double b): x(a), y(b) {}
    vec_2d& operator+=(const vec_2d& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    vec_2d& operator*=(double a) {
        x *= a;
        y *= a;
        return *this;
    }
    vec_2d& operator/=(double a) {
        a = 1 / a;
        return (*this *= a);
    }
};

point_2d operator+(const point_2d& p, const vec_2d& v);
point_2d operator+(const vec_2d& v, const point_2d& p);

vec mult_vec(const vec &v1, const vec &v2);

struct cut final {
    point p;
    vec v;
    cut(const point &p1, const point &p2): p(p1), v(p1, p2) {}
    cut(const point &p_in, const vec &v_in): p(p_in), v(v_in) {}
    point p_end() const {
        return p + v;
    }
    double length() const {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }
};

struct cut_2d final {
    point_2d p;
    vec_2d v;
    cut_2d(const point_2d &p1, const point_2d &p2): p(p1), v(p1, p2) {}
    cut_2d(const point_2d &p_in, const vec_2d &v_in): p(p_in), v(v_in) {}
    point_2d p_end() const {
        return p + v;
    }
    double length() const {
        return sqrt(v.x * v.x + v.y * v.y);
    }
};

g_obj_pos lines_pos_2d(const cut_2d &c1, const cut_2d &c2);
bool is_cut_2d_intersects(const cut_2d &c1, const cut_2d &c2);

struct plane final {
    double a;
    double b;
    double c;
    double d;

    plane(const point &p1, const point &p2, const point &p3) {
        vec v1(p1, p2);
        vec v2(p1, p3);
        vec n = mult_vec(v1, v2);
        n /= n.length();
        a = n.x;
        b = n.y;
        c = n.z;
        d = (a * p1.x + b * p1.y + c * p1.z) * (-1);
    }

    double point_side_plane(const point &p) const {
        return (p.x * a + p.y * b + p.z * c + d) / sqrt(a * a + b * b + c * c);
    }

    vec normal() const {
        return vec(a, b, c);
    }
};

g_obj_pos planes_pos(const plane &pl1, const plane &pl2);
g_obj_pos cut_and_plane_pos(const plane &pl, const cut &c);

class triangle final {
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

class triangle_2d final {
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

class geometry_object final {
private:
    g_obj_type ty;
    union {
        triangle t;
        cut c;
        point p;
    };
    size_t n;
public:
    geometry_object(g_obj_type character, const triangle &t_in, size_t number): ty(character), t(t_in), n(number) {}
    geometry_object(g_obj_type character, const cut &c_in, size_t number): ty(character), c(c_in), n(number) {}
    geometry_object(g_obj_type character, const point &p_in, size_t number): ty(character), p(p_in), n(number) {}
    size_t ret_number() const {
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

class unknown_geometry_object final {
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
        cut c1(p1, p2);
        cut c2(p1, p3);
        cut c3(p2, p3);
        double l1 = c1.length();
        double l2 = c2.length();
        double l3 = c3.length();
        if((l1 >= l2) && (l1 >= l3)) {
            return c1;
        }
        if((l2 >= l1) && (l2 >= l3)) {
            return c2;
        }
        return c3;
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
bool check_intersection(const triangle &t, const plane &pl, const cut &c);
bool check_intersection(const triangle &t, const point &p);
bool check_intersection(const cut &c1, const cut &c2);
bool check_intersection(const cut &c, const point &p);
bool check_intersection(const point &p1, const point &p2);

class intersection_finder final {
private:
    size_t num_of_objects;
    std::list <geometry_object> l;
    std::vector <bool> is_t_intersects;

    //next three methods exist for checking intersections with three different geometry object types
    //each method gets one (root) object with certain type and other object with any type
    //case for TRIANGLE type of the root object also gets a plane of this triangle
    //all methods get number of the root object as root_num

    void root_triangle_case_check(triangle &t_root, size_t root_num, plane &pl, geometry_object &o) {
        if((is_t_intersects[root_num] == false) ||
           (is_t_intersects[o.ret_number()] == false)) {
            if(o.ret_type() == TRIANGLE) {
                triangle t = o.ret_triangle();
                if(check_intersection(t_root, pl, t)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
            if(o.ret_type() == CUT) {
                cut c = o.ret_cut();
                if(check_intersection(t_root, pl, c)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
            if(o.ret_type() == POINT) {
                point p = o.ret_point();
                if(check_intersection(t_root, p)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
        }
    }
    void root_cut_case_check(cut &c_root, size_t root_num, geometry_object &o) {
        if((is_t_intersects[root_num] == false) ||
           (is_t_intersects[o.ret_number()] == false)) {
            if(o.ret_type() == TRIANGLE) {
                triangle t = o.ret_triangle();
                plane pl = t.make_plane();
                if(check_intersection(t, pl, c_root)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
            if(o.ret_type() == CUT) {
                cut c = o.ret_cut();
                if(check_intersection(c_root, c)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
            if(o.ret_type() == POINT) {
                point p = o.ret_point();
                if(check_intersection(c_root, p)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
        }
    }

    void root_point_case_check(point &p_root, size_t root_num, geometry_object &o) {
        if((is_t_intersects[root_num] == false) ||
           (is_t_intersects[o.ret_number()] == false)) {
            if(o.ret_type() == TRIANGLE) {
                triangle t = o.ret_triangle();
                if(check_intersection(t, p_root)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
            if(o.ret_type() == CUT) {
                cut c = o.ret_cut();
                if(check_intersection(c, p_root)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
            if(o.ret_type() == POINT) {
                point p = o.ret_point();
                if(check_intersection(p_root, p)) {
                    is_t_intersects[root_num] = true;
                    is_t_intersects[o.ret_number()] = true;
                }
            }
        }
    }


    //this method needs to copy list of the geometry object to change it in future
    std::list <geometry_object> list_copy() const {
        return l;
    }
    void find_intersections_body(std::list <geometry_object> l_t) {
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
                    root_triangle_case_check(t_root, root_object.ret_number(), pl, o);
                    }
                }
                if((l_t1.size() < l_t_new.size()) && (l_t2.size() < l_t_new.size())) {
                    l_t_new.clear();
                    find_intersections_body(l_t1);
                    find_intersections_body(l_t2);
                }
                else {
                    l_t1.clear();
                    l_t2.clear();
                    find_intersections_body(l_t_new);
                }
            }
            if(root_object.ret_type() == CUT) {
                cut c_root = root_object.ret_cut();
                std::list <geometry_object> l_t_new;
                while(l_t.size() > 0) {
                    geometry_object o = l_t.front();
                    l_t.pop_front();
                    l_t_new.push_back(o);
                    root_cut_case_check(c_root, root_object.ret_number(), o);
                }
                find_intersections_body(l_t_new);
            }
            if(root_object.ret_type() == POINT) {
                point p_root = root_object.ret_point();
                std::list <geometry_object> l_t_new;
                while(l_t.size() > 0) {
                    geometry_object o = l_t.front();
                    l_t.pop_front();
                    l_t_new.push_back(o);
                    root_point_case_check(p_root, root_object.ret_number(), o);
                }
                find_intersections_body(l_t_new);
            }
        }
    }

public:
    intersection_finder(size_t n_t, std::list <geometry_object> &objects): num_of_objects(n_t), l(objects) {
        for(size_t i = 0; i < num_of_objects; i++) {
            is_t_intersects.push_back(false);
        }
    }

    intersection_finder(std::list <geometry_object> &objects): l(objects) {
        num_of_objects = objects.size();
        for(size_t i = 0; i < num_of_objects; i++) {
            is_t_intersects.push_back(false);
        }
    }

    void find_intersections(){
        find_intersections_body(list_copy());
    }

    std::vector <size_t> intersected_objects_nums() const {
        std::vector <size_t> nums;
        for(size_t i = 0; i < num_of_objects; i++) {
            if(is_t_intersects[i] == true) {
                nums.push_back(i);
            }
        }
        return nums;
    }

    //this method returns vector of pairs of:
    //1. triangle geometry objects
    //2. is_intersects for this geometry object (bool type)
    std::vector <std::pair <geometry_object, bool>> objs_condition_only_triangles() const {
        std::vector <std::pair <geometry_object, bool>> objs;
        std::list <geometry_object> l_copy = list_copy();
        while(l_copy.size() > 0) {
            if(l_copy.front().ret_type() == TRIANGLE) {
                std::pair <geometry_object, bool> p(l_copy.front(), is_t_intersects[l_copy.front().ret_number()]);
                objs.push_back(p);
            }
            l_copy.pop_front();
        }
        return objs;
    }
};

}
