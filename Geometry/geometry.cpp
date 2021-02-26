#include <cmath>
#include <cassert>
#include <vector>
#include <stdexcept>

#include "geometry.h"

namespace geometry {

//-------------------------------------------Cut-----------------------------------

void Cut::print() const {
    point p_e(p_end());
    std::cout << p_.x() << " " << p_.y() << " " << p_.z() << std::endl;
    std::cout << p_e.x() << " " << p_e.y() << " " << p_e.z() << std::endl;
}

g_obj_pos lines_pos_2d(const Cut_2d &c1, const Cut_2d &c2) {
    if(vec_2d::is_parallel(c1.vec(), c2.vec()) == false) { return COMMON; }

    vec_2d v_1_to_2(c1.p_begin(), c2.p_begin());
    if(vec_2d::is_parallel(c1.vec(), v_1_to_2) == false) { return PARALLEL; }

    return MATCH;
}

bool is_cut_2d_intersects(const Cut_2d &c1, const Cut_2d &c2) {
    g_obj_pos ind;

    ind = lines_pos_2d(c1, c2);
    if(ind == PARALLEL) {
        return false;
    }

    if(ind == MATCH) {
        double a1, a2;
        if(fabs(c1.vec().x()) > DOUBLE_GAP) {
            a1 = (c2.p_begin().x() - c1.p_begin().x()) / c1.vec().x();
            a2 = (c2.p_end().x() - c1.p_begin().x()) / c1.vec().x();
        }
        else {
            assert(fabs(c1.vec().y()) > DOUBLE_GAP);
            a1 = (c2.p_begin().y() - c1.p_begin().y()) / c1.vec().y();
            a2 = (c2.p_end().y() - c1.p_begin().y()) / c1.vec().y();
        }
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
    a = ((c1.p_begin().x() - c2.p_begin().x()) * c2.vec().y() -
         (c1.p_begin().y() - c2.p_begin().y()) * c2.vec().x()) /
        (c1.vec().y() * c2.vec().x() - c1.vec().x() * c2.vec().y());
    b = ((c2.p_begin().x() - c1.p_begin().x()) * c1.vec().y() -
         (c2.p_begin().y() - c1.p_begin().y()) * c1.vec().x()) /
        (c2.vec().y() * c1.vec().x() - c2.vec().x() * c1.vec().y());
    if((a >= 0) && (a <= 1) && (b >= 0) && (b <= 1)) {
        return true;
    }
    else {
        return false;
    }
}




//-----------------------------------------Plane-----------------------------------

Plane::Plane(const point &p1, const point &p2, const point &p3) {
    vec v1(p1, p2);
    vec v2(p1, p3);
    vec n = mult_vec(v1, v2);
    if(n.is_null()) throw std::invalid_argument("Points for plane on the same line");
    n.normalize();
    a = n.x();
    b = n.y();
    c = n.z();
    d = (a * p1.x() + b * p1.y() + c * p1.z()) * (-1);
}

g_obj_pos planes_pos(const Plane &pl1, const Plane &pl2) {
    if((fabs(pl1.A() - pl2.A()) > DOUBLE_GAP) ||
       (fabs(pl1.B() - pl2.B()) > DOUBLE_GAP)) {
        return COMMON;
    }

    if(abs(pl1.D() - pl2.D()) > DOUBLE_GAP) {
        return PARALLEL;
    }

    return MATCH;
}

g_obj_pos cut_and_plane_pos(const Plane &pl, const Cut &c) {
    if(is_vec_parallel_plane(pl, c.vec()) == false) {
        return COMMON;
    }

    if(is_point_on_plane(pl, c.p_begin()) == false) {
        return PARALLEL;
    }

    return MATCH;
}

point intersection_plane_and_line(const Plane &pl, const Cut &c) {
    if(is_vec_parallel_plane(pl, c.vec())) {
        if(is_point_on_plane(pl, c.p_begin())) { return c.p_begin(); }
        else throw std::invalid_argument("Cut and plane are parallel");
    }

    double k, t, x, y, z;
    const vec& v = c.vec();
    const point& p = c.p_begin();
    k = pl.A() * v.x() + pl.B() * v.y() + pl.C() * v.z();
    t = pl.A() * p.x() + pl.B() * p.y() + pl.C() * p.z() + pl.D();
    t = (-1) * t / k;
    x = p.x() + t * v.x();
    y = p.y() + t * v.y();
    z = p.z() + t * v.z();
    return point(x, y, z);
}

int Plane::point_side_plane(const point& p) const {
    double k = p.x() * a + p.y() * b + p.z() * c + d;
    if(k > DOUBLE_GAP) return 1;
    if(k < -DOUBLE_GAP) return -1;
    return 0;
}

int Plane::cut_side_plane(const Cut& c) const {
    int i1 = point_side_plane(c.p_begin());
    int i2 = point_side_plane(c.p_end());
    if((i1 == 1) && (i2 == 1)) return 1;
    if((i1 == -1) && (i2 == -1)) return -1;
    return 0;
}

int Plane::triangle_side_plane(const Triangle& t) const {
    int i1 = point_side_plane(t.p1());
    int i2 = point_side_plane(t.p2());
    int i3 = point_side_plane(t.p3());
    if((i1 == 1) && (i2 == 1) && (i3 == 1)) return 1;
    if((i1 == -1) && (i2 == -1) && (i3 == -1)) return -1;
    return 0;
}

bool is_vec_parallel_plane(const Plane &pl, const vec &v) {
    return (fabs(v.x() * pl.A() +
                 v.y() * pl.B() +
                 v.z() * pl.C()) < DOUBLE_GAP);
}

bool is_point_on_plane(const Plane &pl, const point& p) {
    return (fabs(p.x() * pl.A() +
                 p.y() * pl.B() +
                 p.z() * pl.C() +
                 pl.D()) < DOUBLE_GAP);
}

//-------------------------------------------Triangle------------------------------

bool Triangle_2d::is_in_triangle(const point_2d &p) const {
    vec_2d v1(p1(), p2());
    vec_2d v2(p1(), p);
    double a1 = v1.x() * v2.y() - v1.y() * v2.x();
    v1 = vec_2d(p2(), p3());
    v2 = vec_2d(p2(), p);
    double a2 = v1.x() * v2.y() - v1.y() * v2.x();
    v1 = vec_2d(p3(), p1());
    v2 = vec_2d(p3(), p);
    double a3 = v1.x() * v2.y() - v1.y() * v2.x();

    if(a1 * a2 < 0) {
        return false;
    }
    if(a2 * a3 < 0) {
        return false;
    }
    return true;
}

bool is_cut_and_triangle_intersects_on_plane(const Triangle &t, const Cut &c) {
    //moving to 2d with checking case if pl is parallel to coordinate plane
    //c lies on pl certainly or UB
    const Plane& pl = t.pl();

    if((fabs(pl.A()) <= fabs(pl.C())) && (fabs(pl.B()) <= fabs(pl.C()))) {
        point_2d p1_2d(t.p1().x(), t.p1().y());
        point_2d p2_2d(t.p2().x(), t.p2().y());
        point_2d p3_2d(t.p3().x(), t.p3().y());
        Triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        Cut_2d c_2d(point_2d(c.p_begin().x(), c.p_begin().y()),
                    vec_2d(c.vec().x(), c.vec().y()));
        return is_cut_and_triangle_intersects_2d(t_2d, c_2d);
    }

    else if((fabs(pl.A()) <= fabs(pl.B())) && (fabs(pl.C()) <= fabs(pl.B()))) {
        point_2d p1_2d(t.p1().x(), t.p1().z());
        point_2d p2_2d(t.p2().x(), t.p2().z());
        point_2d p3_2d(t.p3().x(), t.p3().z());
        Triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        Cut_2d c_2d(point_2d(c.p_begin().x(), c.p_begin().z()),
                    vec_2d(c.vec().x(), c.vec().z()));
        return is_cut_and_triangle_intersects_2d(t_2d, c_2d);
    }

    else {
        point_2d p1_2d(t.p1().y(), t.p1().z());
        point_2d p2_2d(t.p2().y(), t.p2().z());
        point_2d p3_2d(t.p3().y(), t.p3().z());
        Triangle_2d t_2d(p1_2d, p2_2d, p3_2d);
        Cut_2d c_2d(point_2d(c.p_begin().y(), c.p_begin().z()),
                    vec_2d(c.vec().y(), c.vec().z()));
        return is_cut_and_triangle_intersects_2d(t_2d, c_2d);
    }
}

bool is_cut_and_triangle_intersects_2d(const Triangle_2d &t, const Cut_2d &c) {
    //check points lie into triangle
    point_2d p = c.p_begin();
    if(t.is_in_triangle(p)) {
        return true;
    }
    p = c.p_end();
    if(t.is_in_triangle(p)) {
        return true;
    }

    //searching for cuts intersection
    if(is_cut_2d_intersects(c, Cut_2d(t.p1(), t.p2()))) return true;
    if(is_cut_2d_intersects(c, Cut_2d(t.p1(), t.p3()))) return true;
    //third cut is excess

    return false;
}

bool is_triangles_intersects_on_plane(const Triangle &t1, const Triangle &t2) {
    //triangles must be on the same plane or UB
    //moving to 2d with checking case if pl is parallel to coordinate plane
    const Plane& pl = t1.pl();

    if((fabs(pl.A()) <= fabs(pl.C())) && (fabs(pl.B()) <= fabs(pl.C()))) {
        Triangle_2d t1_2d(point_2d(t1.p1().x(), t1.p1().y()),
                          point_2d(t1.p2().x(), t1.p2().y()),
                          point_2d(t1.p3().x(), t1.p3().y()));
        Triangle_2d t2_2d(point_2d(t2.p1().x(), t2.p1().y()),
                          point_2d(t2.p2().x(), t2.p2().y()),
                          point_2d(t2.p3().x(), t2.p3().y()));
        return is_triangles_intersects_2d(t1_2d, t2_2d);
    }

    else if((fabs(pl.A()) <= fabs(pl.B())) && (fabs(pl.C()) <= fabs(pl.B()))) {
        Triangle_2d t1_2d(point_2d(t1.p1().x(), t1.p1().z()),
                          point_2d(t1.p2().x(), t1.p2().z()),
                          point_2d(t1.p3().x(), t1.p3().z()));
        Triangle_2d t2_2d(point_2d(t2.p1().x(), t2.p1().z()),
                          point_2d(t2.p2().x(), t2.p2().z()),
                          point_2d(t2.p3().x(), t2.p3().z()));
        return is_triangles_intersects_2d(t1_2d, t2_2d);
    }

    else {
        Triangle_2d t1_2d(point_2d(t1.p1().y(), t1.p1().z()),
                          point_2d(t1.p2().y(), t1.p2().z()),
                          point_2d(t1.p3().y(), t1.p3().z()));
        Triangle_2d t2_2d(point_2d(t2.p1().y(), t2.p1().z()),
                          point_2d(t2.p2().y(), t2.p2().z()),
                          point_2d(t2.p3().y(), t2.p3().z()));
        return is_triangles_intersects_2d(t1_2d, t2_2d);
    }
}

bool is_triangles_intersects_2d(const Triangle_2d &t1, const Triangle_2d &t2) {
    //checking case if triangle corners lie into other triangle
    if(t1.is_in_triangle(t2.p1())) return true;
    if(t1.is_in_triangle(t2.p2())) return true;
    if(t1.is_in_triangle(t2.p3())) return true;

    if(t2.is_in_triangle(t1.p1())) return true;
    if(t2.is_in_triangle(t1.p2())) return true;
    if(t2.is_in_triangle(t1.p3())) return true;

    //searching for cuts intersection
    if(is_cut_2d_intersects(Cut_2d(t1.p1(), t1.p2()), Cut_2d(t2.p1(), t2.p2()))) return true;
    if(is_cut_2d_intersects(Cut_2d(t1.p1(), t1.p3()), Cut_2d(t2.p1(), t2.p2()))) return true;

    if(is_cut_2d_intersects(Cut_2d(t1.p1(), t1.p2()), Cut_2d(t2.p1(), t2.p3()))) return true;
    if(is_cut_2d_intersects(Cut_2d(t1.p1(), t1.p3()), Cut_2d(t2.p1(), t2.p3()))) return true;
    //other combinations is excess

    return false;
}




//------------------------------------Geometry_Objects-------------------------------

void Geometry_Object::define_object() {
    if(obj_type_ != UNDEFINED) {
        return;
    }

    if(is_points_match(u_o_.p1(), u_o_.p2())) {

        if(is_points_match(u_o_.p1(), u_o_.p3())) {
            obj_type_ = POINT;
            p_ = point(u_o_.p1());
            return;
        }

        obj_type_ = CUT;
        c_ = Cut(u_o_.p1(), u_o_.p3());
        return;
    }

    if((is_points_match(u_o_.p1(), u_o_.p3())) ||
       (is_points_match(u_o_.p2(), u_o_.p3()))) {
        obj_type_ = CUT;
        c_ = Cut(u_o_.p1(), u_o_.p2());
        return;
    }

    if(is_points_on_one_line(u_o_.p1(), u_o_.p2(), u_o_.p3())) {
        obj_type_ = CUT;

        Cut c1(u_o_.p1(), u_o_.p2());
        Cut c2(u_o_.p2(), u_o_.p3());
        Cut c3(u_o_.p1(), u_o_.p3());

        double l1 = c1.length();
        double l2 = c2.length();
        double l3 = c3.length();

        if((l1 >= l2) && (l1 >= l3)) {
            c_ = c1;
            return;
        }
        if((l2 >= l3) && (l2 >= l1)) {
            c_ = c2;
            return;
        }
        c_ = c3;
        return;
    }

    obj_type_ = TRIANGLE;
    t_ = Triangle(u_o_.p1(), u_o_.p2(), u_o_.p3());
}

void Geometry_Object::rotate_object(const Cut& axis, double angle) {
    Normalized_Quaternion quat(angle, axis.vec());

    if(obj_type_ == TRIANGLE) {
        vec v(axis.p_begin(), t_.p1());
        v.rotate_vec(quat);
        point p1_new = axis.p_begin() + v;

        v = vec(axis.p_begin(), t_.p2());
        v.rotate_vec(quat);
        point p2_new = axis.p_begin() + v;

        v = vec(axis.p_begin(), t_.p3());
        v.rotate_vec(quat);
        point p3_new = axis.p_begin() + v;

        t_ = Triangle(p1_new, p2_new, p3_new);
        return;
    }

    if(obj_type_ == CUT) {
        vec v(axis.p_begin(), c_.p_begin());
        v.rotate_vec(quat);
        point p_begin_new = axis.p_begin() + v;

        v = c_.vec();
        v.rotate_vec(quat);

        c_ = Cut(p_begin_new, v);
        return;
    }

    if(obj_type_ == POINT) {
        vec v(axis.p_begin(), p_);
        v.rotate_vec(quat);
        p_ = axis.p_begin() + v;
        return;
    }

    assert(obj_type_ == UNDEFINED);
    vec v(axis.p_begin(), u_o_.p1());
    v.rotate_vec(quat);
    point p1_new = axis.p_begin() + v;

    v = vec(axis.p_begin(), u_o_.p2());
    v.rotate_vec(quat);
    point p2_new = axis.p_begin() + v;

    v = vec(axis.p_begin(), u_o_.p3());
    v.rotate_vec(quat);
    point p3_new = axis.p_begin() + v;

    u_o_ = Undefined_Object(p1_new, p2_new, p3_new);
    return;
}

void Geometry_Object::print() const {
    if(obj_type_ == TRIANGLE) {
        t_.print();
        return;
    }

    if(obj_type_ == CUT) {
        c_.print();
        return;
    }

    if(obj_type_ == POINT) {
        p_.print();
        return;
    }

    assert(obj_type_ == UNDEFINED);
    u_o_.print();
}
} //namespace geometry
