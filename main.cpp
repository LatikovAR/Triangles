#include <iostream>
#include <list>
#include <cassert>
#include <vector>

#include "geometry.h"
#include "triangle_test.h"

geometry::triangle input_triangle(int n);
void print_triangle(const geometry::triangle &t);
void check_t_intersections(std::list <geometry::triangle> &l_t, std::vector <bool> &is_t_intersects);
int triangle_side_by_plane(const geometry::triangle &t, const geometry::plane &pl);
bool check_intersection(const geometry::triangle &t, const geometry::plane &pl, const geometry::triangle &t1);
bool check_t_on_match_pl(const geometry::plane &pl, const geometry::triangle &t1, const geometry::triangle &t2);
bool check_t_intersection_2d(const geometry::triangle_2d &t1, const geometry::triangle_2d &t2);
bool check_t_and_c_intersection_2d(const geometry::triangle_2d &t, const geometry::cut_2d &c);
bool check_common_way_intersection(const geometry::triangle &t, const geometry::plane &pl, const geometry::triangle &t1);
bool check_cut_and_triangle_intersection_on_place(const geometry::triangle &t, const geometry::plane &pl, const geometry::cut &c);

void test_triangle_and_plane() {
    std::cout << "Input points:\n";
    geometry::triangle t = input_triangle(0);
    geometry::plane pl = t.make_plane();
    std::cout << "Plane: " << pl.a << " * x + " << pl.b << " * y + " << pl.c << " * z + " << pl.d << "\n";
    std::cout << "Point: ";
    double x, y, z;
    std::cin >> x >> y >> z;
    geometry::point p(x, y, z);
    std::cout << pl.point_side_plane(p) << "\n";
}

void small_test(std::list <geometry::triangle> *l_t) {
    geometry::vec v1(1, 1, 1);
    geometry::vec v2(10, -10, 10);
    geometry::vec v3(-0.01, 1.1, 0.245);
    geometry::point p(0, 0, 0);
    make_octahedron(l_t, v1, v2, v3, p);
}

void test1(std::list <geometry::triangle> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.01, 1);
}

void test2(std::list <geometry::triangle> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point(1, 1, 0);
    p2 = geometry::point(1, 2, 0);
    p3 = geometry::point(1, 1, 0.03);
    t = geometry::triangle(p1, p2, p3, (int) l_t->size());
    l_t->push_back(t);
}

void test3(std::list <geometry::triangle> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point(1, 1, 0);
    p2 = geometry::point(1, 2, 0);
    p3 = geometry::point(1, 1, 0.03);
    t = geometry::triangle(p1, p2, p3, (int) l_t->size());
    l_t->push_back(t);
    p1 = geometry::point(1, 1, 0.5);
    p2 = geometry::point(1, 2, 0.45);
    p3 = geometry::point(1, 1, 0.66);
    t = geometry::triangle(p1, p2, p3, (int) l_t->size());
    l_t->push_back(t);
}

void test4(std::list <geometry::triangle> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point (0, 0, 0);
    p2 = geometry::point (10, 0, 0);
    p3 = geometry::point (0, 0, 0.5);
    t = geometry::triangle(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.05, -1);
}

void test5(std::list <geometry::triangle> *l_t) {
    geometry::point p1(0, 0, 0);
    geometry::point p2(10, 0, 0);
    geometry::point p3(0, 10, 0);
    geometry::triangle t(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.01, 1);
    p1 = geometry::point (0, 0, 0);
    p2 = geometry::point (10, 0, 0);
    p3 = geometry::point (0, -10, 0);
    t = geometry::triangle(p1, p2, p3, 0);
    make_triangle_stack(l_t, t, 0.01, -1);
}



geometry::triangle input_triangle(int n) {
    double x, y, z;
    std::cin >> x >> y >> z;
    geometry::point p1(x, y, z);
    std::cin >> x >> y >> z;
    geometry::point p2(x, y, z);
    std::cin >> x >> y >> z;
    geometry::point p3(x, y, z);
    geometry::triangle t(p1, p2, p3, n);
    return t;
}

void print_triangle(const geometry::triangle &t) {
    geometry::point p = t.p1_ret();
    std::cout << p.x << " " << p.y << " " << p.z << "\n";
    p = t.p2_ret();
    std::cout << p.x << " " << p.y << " " << p.z << "\n";
    p = t.p3_ret();
    std::cout << p.x << " " << p.y << " " << p.z << "\n";
    //std::cout << t.num() << "\n";
    std::cout << "\n";
}

int main() {
    int n;
    std::vector <bool> is_t_intersects;
    std::list <geometry::triangle> l_t;
    std::cin >> n;
    //small_test(&l_t);
    //test1(&l_t);
    //test2(&l_t);
    //test3(&l_t);
    //test4(&l_t);
    //test5(&l_t);

    for(int i = 0; i < n; i++) {
        l_t.push_back(input_triangle(i));
    }
    assert((int) l_t.size() == n);

    n = (int) l_t.size();
    for(int i = 0; i < n; i++) {
        is_t_intersects.push_back(false);
    }
    check_t_intersections(l_t, is_t_intersects);

    for(int i = 0; i < n; i++) {
        if(is_t_intersects[(size_t) i] == true) {
            std::cout << i << "\n";
        }
    }
    return 0;
}

void check_t_intersections(std::list <geometry::triangle> &l_t, std::vector <bool> &is_t_intersects) {
    assert(l_t.size() > 0);
    geometry::triangle t = l_t.front();
    l_t.pop_front();
    if(l_t.size() > 0) {
        std::list <geometry::triangle> l_t1;
        std::list <geometry::triangle> l_t2;
        geometry::plane pl = t.make_plane();
        int ind;
        while(l_t.size() > 0) {
            ind = triangle_side_by_plane(l_t.front(), pl);
            //return triangle position relative to plane
            //0 - intersect, 1 and -1 - one or another side entirely
            if(ind == -1) {
                l_t1.push_front(l_t.front());
            }
            if(ind == 1) {
                l_t2.push_front(l_t.front());
            }
            if(ind == 0) {
                l_t1.push_front(l_t.front());
                l_t2.push_front(l_t.front());
                if(((is_t_intersects[(size_t) t.num()] == false) ||
                  (is_t_intersects[(size_t) l_t.front().num()] == false)) &&
                  (check_intersection(t, pl, l_t.front()))) {
                    is_t_intersects[(size_t) t.num()] = true;
                    is_t_intersects[(size_t) l_t.front().num()] = true;
                }
            }
            l_t.pop_front();
        }
        if(l_t1.size() > 0) {
            check_t_intersections(l_t1, is_t_intersects); //recursion
        }
        if(l_t2.size() > 0) {
            check_t_intersections(l_t2, is_t_intersects); //recursion
        }
    }
}

int triangle_side_by_plane(const geometry::triangle &t, const geometry::plane &pl) {
    double i1, i2, i3;
    geometry::point p = t.p1_ret();
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

bool check_intersection(const geometry::triangle &t, const geometry::plane &pl, const geometry::triangle &t1) {
    geometry::plane pl1 = t1.make_plane();
    geometry::g_obj_pos p_pos = planes_pos(pl, pl1);
    if(p_pos == geometry::PARALLEL) {
        return false;
    }
    if(p_pos == geometry::MATCH) {
        return check_t_on_match_pl(pl, t, t1);
    }
    if(p_pos == geometry::COMMON) {
        return check_common_way_intersection(t, pl, t1);
    }
    assert(1);
    return false;
}

bool check_t_on_match_pl(const geometry::plane &pl, const geometry::triangle &t1, const geometry::triangle &t2) {
    //moving to 2d with checking case if pl is parallel to coordinate plane
    if((abs(pl.a) <= abs(pl.c)) && (abs(pl.b) <= abs(pl.c))) {
        geometry::point_2d p1_2d(t1.p1_ret().x, t1.p1_ret().y);
        geometry::point_2d p2_2d(t1.p2_ret().x, t1.p2_ret().y);
        geometry::point_2d p3_2d(t1.p3_ret().x, t1.p3_ret().y);
        geometry::triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d = geometry::point_2d(t2.p1_ret().x, t2.p1_ret().y);
        p2_2d = geometry::point_2d(t2.p2_ret().x, t2.p2_ret().y);
        p3_2d = geometry::point_2d(t2.p3_ret().x, t2.p3_ret().y);
        geometry::triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
    else if((abs(pl.a) <= abs(pl.b)) && (abs(pl.c) <= abs(pl.b))) {
        geometry::point_2d p1_2d(t1.p1_ret().x, t1.p1_ret().z);
        geometry::point_2d p2_2d(t1.p2_ret().x, t1.p2_ret().z);
        geometry::point_2d p3_2d(t1.p3_ret().x, t1.p3_ret().z);
        geometry::triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d = geometry::point_2d(t2.p1_ret().x, t2.p1_ret().z);
        p2_2d = geometry::point_2d(t2.p2_ret().x, t2.p2_ret().z);
        p3_2d = geometry::point_2d(t2.p3_ret().x, t2.p3_ret().z);
        geometry::triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
    else {
        geometry::point_2d p1_2d(t1.p1_ret().y, t1.p1_ret().z);
        geometry::point_2d p2_2d(t1.p2_ret().y, t1.p2_ret().z);
        geometry::point_2d p3_2d(t1.p3_ret().y, t1.p3_ret().z);
        geometry::triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d = geometry::point_2d(t2.p1_ret().y, t2.p1_ret().z);
        p2_2d = geometry::point_2d(t2.p2_ret().y, t2.p2_ret().z);
        p3_2d = geometry::point_2d(t2.p3_ret().y, t2.p3_ret().z);
        geometry::triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
}

bool check_common_way_intersection(const geometry::triangle &t, const geometry::plane &pl, const geometry::triangle &t1) {
    std::vector <geometry::point> arr_p;
    geometry::g_obj_pos c1_ind, c2_ind, c3_ind;

    geometry::point p1 = t1.p1_ret();
    geometry::point p2 = t1.p2_ret();
    geometry::cut c1(p1, p2);
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    geometry::cut c2(p1, p2);
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    geometry::cut c3(p1, p2);

    c1_ind = cut_and_plane_pos(pl, c1);
    c2_ind = cut_and_plane_pos(pl, c2);
    c3_ind = cut_and_plane_pos(pl, c3);
    //cases if some t1 cut lies on pl
    if(c1_ind == geometry::MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c1);
    }
    if(c2_ind == geometry::MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c2);
    }
    if(c3_ind == geometry::MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c3);
    }

    //common way: searching two points which t1 cuts intersect pl
    p1 = t1.p1_ret();
    p2 = t1.p2_ret();
    if((c1_ind == geometry::COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        arr_p.push_back(intersection_pc(pl, c1));
    }
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    if((c2_ind == geometry::COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        arr_p.push_back(intersection_pc(pl, c2));
    }
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    if((arr_p.size() < 2) && (c3_ind == geometry::COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        arr_p.push_back(intersection_pc(pl, c3));
    }
    assert(arr_p.size() == 2);

    //case if triangle corner lies on pl
    if(is_points_match(arr_p[0], arr_p[1])) {
        return t.is_in_triangle(arr_p[0]);
    }

    geometry::cut c(arr_p[0], arr_p[1]);
    return check_cut_and_triangle_intersection_on_place(t, pl, c);
}

bool check_cut_and_triangle_intersection_on_place(const geometry::triangle &t, const geometry::plane &pl, const geometry::cut &c) {
    //moving to 2d with checking case if pl is parallel to coordinate plane
    //c lies on pl certainly
    if((abs(pl.a) <= abs(pl.c)) && (abs(pl.b) <= abs(pl.c))) {
        geometry::point_2d p1_2d(t.p1_ret().x, t.p1_ret().y);
        geometry::point_2d p2_2d(t.p2_ret().x, t.p2_ret().y);
        geometry::point_2d p3_2d(t.p3_ret().x, t.p3_ret().y);
        geometry::triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        geometry::vec_2d v_2d(c.v.x, c.v.y);
        p1_2d = geometry::point_2d(c.p.x, c.p.y);
        geometry::cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
    else if((abs(pl.a) <= abs(pl.b)) && (abs(pl.c) <= abs(pl.b))) {
        geometry::point_2d p1_2d(t.p1_ret().x, t.p1_ret().z);
        geometry::point_2d p2_2d(t.p2_ret().x, t.p2_ret().z);
        geometry::point_2d p3_2d(t.p3_ret().x, t.p3_ret().z);
        geometry::triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        geometry::vec_2d v_2d(c.v.x, c.v.z);
        p1_2d = geometry::point_2d(c.p.x, c.p.z);
        geometry::cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
    else {
        geometry::point_2d p1_2d(t.p1_ret().y, t.p1_ret().z);
        geometry::point_2d p2_2d(t.p2_ret().y, t.p2_ret().z);
        geometry::point_2d p3_2d(t.p3_ret().y, t.p3_ret().z);
        geometry::triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        geometry::vec_2d v_2d(c.v.y, c.v.z);
        p1_2d = geometry::point_2d(c.p.y, c.p.z);
        geometry::cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
}

bool check_t_intersection_2d(const geometry::triangle_2d &t1, const geometry::triangle_2d &t2) {
    //checking case if triangle corners lie into other triangle
    geometry::point_2d p1 = t2.p1_ret();
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
    geometry::point_2d p2 = t1.p2_ret();
    geometry::cut_2d c11(p1, p2); // why cut_2d c11(t1.p1_ret(), t1.p2_ret()); not works?
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    geometry::cut_2d c12(p1, p2);
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    geometry::cut_2d c13(p1, p2);

    p1 = t2.p1_ret();
    p2 = t2.p2_ret();
    geometry::cut_2d c21(p1, p2);
    if(is_cut_2d_intersects(c11, c21)) return true;
    if(is_cut_2d_intersects(c12, c21)) return true;
    if(is_cut_2d_intersects(c13, c21)) return true;

    p1 = t2.p1_ret();
    p2 = t2.p3_ret();
    geometry::cut_2d c22(p1, p2);
    if(is_cut_2d_intersects(c11, c22)) return true;
    if(is_cut_2d_intersects(c12, c22)) return true;
    if(is_cut_2d_intersects(c13, c22)) return true;

    p1 = t2.p2_ret();
    p2 = t2.p3_ret();
    geometry::cut_2d c23(p1, p2);
    if(is_cut_2d_intersects(c11, c23)) return true;
    if(is_cut_2d_intersects(c12, c23)) return true;
    if(is_cut_2d_intersects(c13, c23)) return true; //it is excess

    return false;
}

bool check_t_and_c_intersection_2d(const geometry::triangle_2d &t, const geometry::cut_2d &c) {
    //check points lie into triangle
    geometry::point_2d p1 = c.p;
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
    geometry::point_2d p2 = t.p2_ret();
    geometry::cut_2d c_t1(p1, p2);
    if(is_cut_2d_intersects(c, c_t1)) return true;
    p1 = t.p1_ret();
    p2 = t.p3_ret();
    geometry::cut_2d c_t2(p1, p2);
    if(is_cut_2d_intersects(c, c_t2)) return true;
    p1 = t.p2_ret();
    p2 = t.p3_ret();
    geometry::cut_2d c_t3(p1, p2);
    if(is_cut_2d_intersects(c, c_t3)) return true; //excess
    return false;
}
