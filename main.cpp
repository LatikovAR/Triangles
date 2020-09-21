#include <iostream>
#include <list>
#include <cassert>

#include "geometry.h"

triangle input_triangle(int n);
void check_t_intersections(std::list <triangle> &l_t, bool *is_t_intersects);
int triangle_side_by_plane(triangle &t, plane &pl);
bool check_intersection(triangle &t, plane &pl, triangle &t1);
bool check_t_on_match_pl(plane &pl, triangle &t1, triangle &t2);
bool check_t_intersection_2d(triangle_2d &t1, triangle_2d &t2);
bool check_t_and_c_intersection_2d(triangle_2d &t, cut_2d &c);
bool check_common_way_intersection(triangle &t, plane &pl, triangle &t1);
bool check_cut_and_triangle_intersection_on_place(triangle &t, plane &pl, cut &c);

void test_triangle_and_plane() {
    std::cout << "Input points:\n";
    triangle t = input_triangle(0);
    plane pl = t.make_plane();
    std::cout << "Plane: " << pl.a << " * x + " << pl.b << " * y + " << pl.c << " * z + " << pl.d << "\n";
    std::cout << "Point: ";
    point p;
    std::cin >> p.x >> p.y >> p.z;
    std::cout << pl.point_side_plane(p) << "\n";
}

triangle input_triangle(int n) {
    point p1, p2, p3;
    std::cin >> p1.x >> p1.y >> p1.z;
    std::cin >> p2.x >> p2.y >> p2.z;
    std::cin >> p3.x >> p3.y >> p3.z;
    triangle t(p1, p2, p3, n);
    return t;
}

int main() {
    int n;
    bool *is_t_intersects;
    std::cin >> n;
    is_t_intersects = new bool[n];
    assert(is_t_intersects != nullptr);
    for(int i = 0; i < n; i++) {
        is_t_intersects[i] = false;
    }
    std::list <triangle> l_t;
    for(int i = 0; i < n; i++) {
        l_t.push_back(input_triangle(i));
    }
    assert((int) l_t.size() == n);
    check_t_intersections(l_t, is_t_intersects);
    int counter = 0;
    for(int i = 0; i < n; i++) {
        if(is_t_intersects[i] == true) {
            counter++;
        }
    }
    std::cout << "Number of intersected triangles: " << counter << "\n";
    delete [] is_t_intersects;
    return 0;
}

void check_t_intersections(std::list <triangle> &l_t, bool *is_t_intersects) {
    assert(is_t_intersects != nullptr);
    assert(l_t.size() > 0);
    triangle t = l_t.front();
    l_t.pop_front();
    if(l_t.size() > 0) {
        std::list <triangle> l_t1;
        std::list <triangle> l_t2;
        plane pl = t.make_plane();
        int ind;
        while(l_t.size() > 0) {
            ind = triangle_side_by_plane(l_t.front(), pl);
            if(ind == -1) {
                l_t1.push_front(l_t.front());
            }
            if(ind == 1) {
                l_t2.push_front(l_t.front());
            }
            if(ind == 0) {
                l_t1.push_front(l_t.front());
                l_t2.push_front(l_t.front());
                if(((is_t_intersects[t.num()] == false) ||
                  (is_t_intersects[l_t.front().num()] == false)) &&
                  (check_intersection(t, pl, l_t.front()))) {
                    is_t_intersects[t.num()] = true;
                    is_t_intersects[l_t.front().num()] = true;
                }
            }
            l_t.pop_front();
        }
        if(l_t1.size() > 0) {
            check_t_intersections(l_t1, is_t_intersects);
        }
        if(l_t2.size() > 0) {
            check_t_intersections(l_t2, is_t_intersects);
        }
    }
}

int triangle_side_by_plane(triangle &t, plane &pl) {
    double i1, i2, i3;
    point p;
    p = t.p1_ret();
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

bool check_intersection(triangle &t, plane &pl, triangle &t1) {
    plane pl1 = t1.make_plane();
    g_obj_pos p_pos = planes_pos(pl, pl1);
    if(p_pos == PARALLEL) {
        return false;
    }
    if(p_pos == MATCH) {
        return check_t_on_match_pl(pl, t, t1);
    }
    if(p_pos == COMMON) {
        return check_common_way_intersection(t, pl, t1);
    }
    assert(1);
    return false;
}

bool check_t_on_match_pl(plane &pl, triangle &t1, triangle &t2) {
    point_2d p1_2d, p2_2d, p3_2d;
    if((abs(pl.a) <= abs(pl.c)) && (abs(pl.b) <= abs(pl.c))) {
        p1_2d.x = t1.p1_ret().x;
        p1_2d.y = t1.p1_ret().y;
        p2_2d.x = t1.p2_ret().x;
        p2_2d.y = t1.p2_ret().y;
        p3_2d.x = t1.p3_ret().x;
        p3_2d.y = t1.p3_ret().y;
        triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d.x = t2.p1_ret().x;
        p1_2d.y = t2.p1_ret().y;
        p2_2d.x = t2.p2_ret().x;
        p2_2d.y = t2.p2_ret().y;
        p3_2d.x = t2.p3_ret().x;
        p3_2d.y = t2.p3_ret().y;
        triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
    else if((abs(pl.a) <= abs(pl.b)) && (abs(pl.c) <= abs(pl.b))) {
        p1_2d.x = t1.p1_ret().x;
        p1_2d.y = t1.p1_ret().z;
        p2_2d.x = t1.p2_ret().x;
        p2_2d.y = t1.p2_ret().z;
        p3_2d.x = t1.p3_ret().x;
        p3_2d.y = t1.p3_ret().z;
        triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d.x = t2.p1_ret().x;
        p1_2d.y = t2.p1_ret().z;
        p2_2d.x = t2.p2_ret().x;
        p2_2d.y = t2.p2_ret().z;
        p3_2d.x = t2.p3_ret().x;
        p3_2d.y = t2.p3_ret().z;
        triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
    else {
        p1_2d.x = t1.p1_ret().y;
        p1_2d.y = t1.p1_ret().z;
        p2_2d.x = t1.p2_ret().y;
        p2_2d.y = t1.p2_ret().z;
        p3_2d.x = t1.p3_ret().y;
        p3_2d.y = t1.p3_ret().z;
        triangle_2d t1_2d(p1_2d, p2_2d, p3_2d);
        p1_2d.x = t2.p1_ret().y;
        p1_2d.y = t2.p1_ret().z;
        p2_2d.x = t2.p2_ret().y;
        p2_2d.y = t2.p2_ret().z;
        p3_2d.x = t2.p3_ret().y;
        p3_2d.y = t2.p3_ret().z;
        triangle_2d t2_2d(p1_2d, p2_2d, p3_2d);
        return check_t_intersection_2d(t1_2d, t2_2d);
    }
}

bool check_common_way_intersection(triangle &t, plane &pl, triangle &t1) {
    point p1, p2, p[2];
    int i;
    g_obj_pos c1_ind, c2_ind, c3_ind;

    p1 = t1.p1_ret();
    p2 = t1.p2_ret();
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

    if(c1_ind == MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c1);
    }
    if(c2_ind == MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c2);
    }
    if(c3_ind == MATCH) {
        return check_cut_and_triangle_intersection_on_place(t, pl, c3);
    }

    i = 0;
    p1 = t1.p1_ret();
    p2 = t1.p2_ret();
    if((c1_ind == COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        p[i] = intersection_pc(pl, c1);
        i++;
    }
    p1 = t1.p1_ret();
    p2 = t1.p3_ret();
    if((c2_ind == COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        p[i] = intersection_pc(pl, c2);
        i++;
    }
    p1 = t1.p2_ret();
    p2 = t1.p3_ret();
    if((i < 2) && (c3_ind == COMMON) && (pl.point_side_plane(p1) * pl.point_side_plane(p2) <= 0)) {
        p[i] = intersection_pc(pl, c3);
        i++;
    }
    assert(i == 2);

    cut c(p[0], p[1]);
    return check_cut_and_triangle_intersection_on_place(t, pl, c);
}

bool check_cut_and_triangle_intersection_on_place(triangle &t, plane &pl, cut &c) {
    point_2d p1_2d, p2_2d, p3_2d;
    vec_2d v_2d;
    if((abs(pl.a) <= abs(pl.c)) && (abs(pl.b) <= abs(pl.c))) {
        p1_2d.x = t.p1_ret().x;
        p1_2d.y = t.p1_ret().y;
        p2_2d.x = t.p2_ret().x;
        p2_2d.y = t.p2_ret().y;
        p3_2d.x = t.p3_ret().x;
        p3_2d.y = t.p3_ret().y;
        triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        p1_2d.x = c.p.x;
        p1_2d.y = c.p.y;
        v_2d.x = c.v.x;
        v_2d.y = c.v.y;
        cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
    else if((abs(pl.a) <= abs(pl.b)) && (abs(pl.c) <= abs(pl.b))) {
        p1_2d.x = t.p1_ret().x;
        p1_2d.y = t.p1_ret().z;
        p2_2d.x = t.p2_ret().x;
        p2_2d.y = t.p2_ret().z;
        p3_2d.x = t.p3_ret().x;
        p3_2d.y = t.p3_ret().z;
        triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        p1_2d.x = c.p.x;
        p1_2d.y = c.p.z;
        v_2d.x = c.v.x;
        v_2d.y =c.v.z;
        cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
    else {
        p1_2d.x = t.p1_ret().y;
        p1_2d.y = t.p1_ret().z;
        p2_2d.x = t.p2_ret().y;
        p2_2d.y = t.p2_ret().z;
        p3_2d.x = t.p3_ret().y;
        p3_2d.y = t.p3_ret().z;
        triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        p1_2d.x = c.p.y;
        p1_2d.y = c.p.z;
        v_2d.x = c.v.y;
        v_2d.y =c.v.z;
        cut_2d c_2d(p1_2d, v_2d);
        return check_t_and_c_intersection_2d(t_2d, c_2d);
    }
}

bool check_t_intersection_2d(triangle_2d &t1, triangle_2d &t2) {
    point_2d p1, p2;

    p1 = t2.p1_ret();
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

    p1 = t1.p1_ret();
    p2 = t1.p2_ret();
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

bool check_t_and_c_intersection_2d(triangle_2d &t, cut_2d &c) {
    point_2d p1, p2;
    p1 = c.p;
    if(t.is_in_triangle(p1)) {
        return true;
    }
    p1.x = c.p.x + c.v.x;
    p1.y = c.p.y + c.v.y;
    if(t.is_in_triangle(p1)) {
        return true;
    }

    p1 = t.p1_ret();
    p2 = t.p2_ret();
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
