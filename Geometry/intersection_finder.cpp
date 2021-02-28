#include <cstdlib>
#include <vector>
#include <typeinfo>

#include "intersection_finder.h"
#include "geometry.h"

namespace geometry {

//-------------------------------------Intersection_Finder------------------------

Intersection_Finder::Intersection_Finder(std::vector<Geometry_Object> objects):
    objects_(std::move(objects))
{
    intersection_flags_.reserve(objects_.size());
    for(size_t i = 0; i < objects_.size(); ++i) {
        intersection_flags_.push_back(false);
    }
}

Objects_and_Intersections Intersection_Finder::compute_intersections() {

    for(size_t i = 0; i < objects_.size(); ++i) {
        objects_[i].define_object();
    }

    std::vector<Geometry_Object*> p_objects(objects_.size());
    for(size_t i = 0; i < objects_.size(); ++i) {
        p_objects[i] = &(objects_[i]);
    }

    compute_intersections_recursive_algorithm(p_objects);

    Objects_and_Intersections answer(std::move(objects_),
                                     std::move(intersection_flags_));
    return answer;
}

void Intersection_Finder::compute_intersections_recursive_algorithm(
        const std::vector<Geometry_Object*>& p_objects)
{
    if(p_objects.size() > 1) {
        Geometry_Object* root_object = p_objects[0];

        assert(root_object->type() != UNDEFINED);

        if(root_object->type() == TRIANGLE) {
            root_triangle_case(p_objects);
            return;
        }

        if(root_object->type() == CUT) {
            root_cut_case(p_objects);
            return;
        }

        assert(root_object->type() == POINT);
        root_point_case(p_objects);
    }
}

void Intersection_Finder::root_triangle_case(const std::vector<Geometry_Object*>& p_objects) {
    const Triangle& root_t = p_objects[0]->triangle();
    const Plane& pl = root_t.pl();
    std::vector<Geometry_Object*> p_objects_new1;
    std::vector<Geometry_Object*> p_objects_new2;

    for(size_t i = 1; i < p_objects.size(); ++i) {
        Geometry_Object* cur_obj = p_objects[i];

        if(cur_obj->type() == TRIANGLE) {
            const Triangle& t = cur_obj->triangle();
            int k = pl.triangle_side_plane(t);

            if(k != 1) p_objects_new2.push_back(cur_obj);
            if(k != -1) p_objects_new1.push_back(cur_obj);

            if(k == 0) {
                if(check_intersection(root_t, t)) {
                    intersection_flags_[p_objects[0]->number()] = true;
                    intersection_flags_[cur_obj->number()] = true;
                }
            }

            continue;
        }

        if(cur_obj->type() == CUT) {
            const Cut& c = cur_obj->cut();
            int k = pl.cut_side_plane(c);

            if(k != 1) p_objects_new2.push_back(cur_obj);
            if(k != -1) p_objects_new1.push_back(cur_obj);

            if(k == 0) {
                if(check_intersection(root_t, c)) {
                    intersection_flags_[p_objects[0]->number()] = true;
                    intersection_flags_[cur_obj->number()] = true;
                }
            }

            continue;
        }

        assert(cur_obj->type() == POINT);
        const point& p = cur_obj->pointt();
        int k = pl.point_side_plane(p);

        if(k != 1) p_objects_new2.push_back(cur_obj);
        if(k != -1) p_objects_new1.push_back(cur_obj);

        if(k == 0) {
            if(check_intersection(root_t, p)) {
                intersection_flags_[p_objects[0]->number()] = true;
                intersection_flags_[cur_obj->number()] = true;
            }
        }
    }

    assert((p_objects_new1.size() < p_objects.size()) &&
           (p_objects_new2.size() < p_objects.size()));


    if(p_objects_new1.size() == (p_objects.size() - 1)) {
        compute_intersections_recursive_algorithm(p_objects_new1);
        return;
    }

    if (p_objects_new2.size() == (p_objects.size() - 1)) {
        compute_intersections_recursive_algorithm(p_objects_new2);
        return;
    }

    compute_intersections_recursive_algorithm(p_objects_new1);
    compute_intersections_recursive_algorithm(p_objects_new2);
}

void Intersection_Finder::root_cut_case(const std::vector<Geometry_Object*>& p_objects) {
    const Cut& root_c = p_objects[0]->cut();
    std::vector<Geometry_Object*> p_objects_new;
    p_objects_new.reserve(p_objects.size() - 1);

    for(size_t i = 1; i < p_objects.size(); ++i) {
        Geometry_Object* cur_obj = p_objects[i];
        p_objects_new.push_back(cur_obj);

        if(cur_obj->type() == TRIANGLE) {
            const Triangle& t = cur_obj->triangle();

            if(check_intersection(root_c, t)) {
                intersection_flags_[p_objects[0]->number()] = true;
                intersection_flags_[cur_obj->number()] = true;
            }

            continue;
        }

        if(cur_obj->type() == CUT) {
            const Cut& c = cur_obj->cut();

            if(check_intersection(root_c, c)) {
                intersection_flags_[p_objects[0]->number()] = true;
                intersection_flags_[cur_obj->number()] = true;
            }

            continue;
        }

        assert(cur_obj->type() == POINT);
        const point& p = cur_obj->pointt();

        if(check_intersection(root_c, p)) {
            intersection_flags_[p_objects[0]->number()] = true;
            intersection_flags_[cur_obj->number()] = true;
        }
    }

    assert(p_objects_new.size() < p_objects.size());

    compute_intersections_recursive_algorithm(p_objects_new);
}

void Intersection_Finder::root_point_case(const std::vector<Geometry_Object*>& p_objects) {
    const point& root_p = p_objects[0]->pointt();
    std::vector<Geometry_Object*> p_objects_new;
    p_objects_new.reserve(p_objects.size() - 1);

    for(size_t i = 1; i < p_objects.size(); ++i) {
        Geometry_Object* cur_obj = p_objects[i];
        p_objects_new.push_back(cur_obj);

        if(cur_obj->type() == TRIANGLE) {
            const Triangle& t = cur_obj->triangle();

            if(check_intersection(root_p, t)) {
                intersection_flags_[p_objects[0]->number()] = true;
                intersection_flags_[cur_obj->number()] = true;
            }

            continue;
        }

        if(cur_obj->type() == CUT) {
            const Cut& c = cur_obj->cut();

            if(check_intersection(root_p, c)) {
                intersection_flags_[p_objects[0]->number()] = true;
                intersection_flags_[cur_obj->number()] = true;
            }

            continue;
        }

        assert(cur_obj->type() == POINT);
        const point& p = cur_obj->pointt();

        if(check_intersection(root_p, p)) {
            intersection_flags_[p_objects[0]->number()] = true;
            intersection_flags_[cur_obj->number()] = true;
        }
    }

    assert(p_objects_new.size() < p_objects.size());

    compute_intersections_recursive_algorithm(p_objects_new);
}

bool Intersection_Finder::check_intersection(const Triangle &t1, const Triangle &t2) {
    g_obj_pos p_pos = planes_pos(t1.pl(), t2.pl());

    if(p_pos == PARALLEL) return false;
    if(p_pos == MATCH) return is_triangles_intersects_on_plane(t1, t2);
    assert(p_pos == COMMON);

    //next two blocks can be deleted, but they give a VERY STRONG optimization
    //checking is t2 have common points with t1.pl()
    const Plane& pl_base1 = t1.pl();
    int i11, i12, i13;
    i11 = pl_base1.point_side_plane(t2.p1());
    i12 = pl_base1.point_side_plane(t2.p2());
    i13 = pl_base1.point_side_plane(t2.p3());
    if((i11 * i12 > 0) && (i12 * i13 > 0)) return false;

    //checking is t1 have common points with t2.pl()
    const Plane& pl_base2 = t2.pl();
    int i21, i22, i23;
    i21 = pl_base2.point_side_plane(t1.p1());
    i22 = pl_base2.point_side_plane(t1.p2());
    i23 = pl_base2.point_side_plane(t1.p3());
    if((i21 * i22 > 0) && (i22 * i23 > 0)) return false;

    Cut c1(t2.p1(), t2.p2());
    Cut c2(t2.p1(), t2.p3());
    Cut c3(t2.p2(), t2.p3());

    g_obj_pos c1_ind, c2_ind, c3_ind;
    c1_ind = cut_and_plane_pos(pl_base1, c1);
    c2_ind = cut_and_plane_pos(pl_base1, c2);
    c3_ind = cut_and_plane_pos(pl_base1, c3);

    //cases if some t2 cut lies on t1.pl()
    if(c1_ind == MATCH) return is_cut_and_triangle_intersects_on_plane(t1, c1);
    if(c2_ind == MATCH) return is_cut_and_triangle_intersects_on_plane(t1, c2);
    if(c3_ind == MATCH) return is_cut_and_triangle_intersects_on_plane(t1, c3);

    //common way: searching two points which t2 cuts intersect t1.pl()
    std::vector <point> arr_p;
    if(i11 * i12 < 0) {
        arr_p.push_back(intersection_plane_and_line(pl_base1, c1));
    }
    if(i11 * i13 < 0) {
        arr_p.push_back(intersection_plane_and_line(pl_base1, c2));
    }
    if(i12 * i13 < 0) {
        arr_p.push_back(intersection_plane_and_line(pl_base1, c3));
    }
    if(i11 == 0) {
        arr_p.push_back(t2.p1());
     }
    if(i12 == 0) {
        arr_p.push_back(t2.p2());
     }
    if(i13 == 0) {
        arr_p.push_back(t2.p3());
     }

    assert((arr_p.size() <= 2) && (arr_p.size() >= 1));

    //case if triangle corner lies on pl
    if(arr_p.size() == 1) {
        return check_intersection(t1, arr_p[0]);
    }

    Cut c(arr_p[0], arr_p[1]);
    return is_cut_and_triangle_intersects_on_plane(t1, c);
}

bool Intersection_Finder::check_intersection(const Triangle &t, const Cut &c) {
    g_obj_pos pos = cut_and_plane_pos(t.pl(), c);

    if(pos == PARALLEL) return false;
    if(pos == MATCH) return is_cut_and_triangle_intersects_on_plane(t, c);
    assert(pos == COMMON);
    point p = intersection_plane_and_line(t.pl(), c);

    const vec& v = c.vect();
    if((v.x() >= v.y()) && (v.x() >= v.z())) {
        double k = (p.x() - c.p_begin().x()) / v.x();
        if((k < 0) || (k > 1)) return false;
    }
    else if((v.y() >= v.z()) && (v.y() >= v.z())) {
        double k = (p.y() - c.p_begin().y()) / v.y();
        if((k < 0) || (k > 1)) return false;
    }
    else {
        double k = (p.z() - c.p_begin().z()) / v.z();
        if((k < 0) || (k > 1)) return false;
    }

    return check_intersection(t, p);
}

bool Intersection_Finder::check_intersection(const Triangle &t, const point &p) {
    if(is_point_on_plane(t.pl(), p) == false) return false;

    vec a1 = mult_vec(vec(t.p1(), t.p2()), vec(t.p1(), p));
    vec a2 = mult_vec(vec(t.p2(), t.p3()), vec(t.p2(), p));
    vec a3 = mult_vec(vec(t.p3(), t.p1()), vec(t.p3(), p));

    if((fabs(a1.x()) >= fabs(a1.y())) && (fabs(a1.x()) >= fabs(a1.z()))) {
        if(a1.x() * a2.x() < 0) return false;
        if(a2.x() * a3.x() < 0) return false;
        return true;
    }

    if((fabs(a1.y()) >= fabs(a1.x())) && (fabs(a1.y()) >= fabs(a1.z()))) {
        if(a1.y() * a2.y() < 0) return false;
        if(a2.y() * a3.y() < 0) return false;
        return true;
    }

    if(a1.z() * a2.z() < 0) return false;
    if(a2.z() * a3.z() < 0) return false;
    return true;
}

bool Intersection_Finder::check_intersection(const Cut &c1, const Cut &c2) {
    if(is_points_on_one_line(c1.p_begin(), c1.p_end(), c2.p_begin())) {
        if(check_intersection(c1, c2.p_begin()) == true) return true;
        else return check_intersection(c1, c2.p_end());
    }

    Plane pl(c1.p_begin(), c1.p_end(), c2.p_begin());
    double i = pl.point_side_plane(c2.p_end());
    if(fabs(i) > DOUBLE_GAP) {
        return false;
    }

    if((fabs(pl.A()) >= fabs(pl.B())) && (fabs(pl.A()) >= fabs(pl.C()))) {
        return is_cut_2d_intersects(Cut_2d(point_2d(c1.p_begin().y(), c1.p_begin().z()),
                                           vec_2d(c1.vect().y(), c1.vect().z())),
                                    Cut_2d(point_2d(c2.p_begin().y(), c2.p_begin().z()),
                                           vec_2d(c2.vect().y(), c2.vect().z())));
    }

    if((fabs(pl.B()) >= fabs(pl.A())) && (fabs(pl.B()) >= fabs(pl.C()))) {
        return is_cut_2d_intersects(Cut_2d(point_2d(c1.p_begin().x(), c1.p_begin().z()),
                                           vec_2d(c1.vect().x(), c1.vect().z())),
                                    Cut_2d(point_2d(c2.p_begin().x(), c2.p_begin().z()),
                                           vec_2d(c2.vect().x(), c2.vect().z())));
    }

    return is_cut_2d_intersects(Cut_2d(point_2d(c1.p_begin().x(), c1.p_begin().y()),
                                       vec_2d(c1.vect().x(), c1.vect().y())),
                                Cut_2d(point_2d(c2.p_begin().x(), c2.p_begin().y()),
                                       vec_2d(c2.vect().x(), c2.vect().y())));
}

bool Intersection_Finder::check_intersection(const Cut &c, const point &p) {
    const vec& v_c = c.vect();
    const vec v_p(c.p_begin(), p);
    if(vec::is_parallel(v_c, v_p) == false) return false;

    if((fabs(v_c.x()) >= fabs(v_c.y())) && (fabs(v_c.x()) >= fabs(v_c.z()))) {
        assert(fabs(v_c.x()) > DOUBLE_GAP);

        double k = v_p.x() / v_c.x();
        if((k < 0) || (k > 1)) return false;
        return true;
    }

    if((fabs(v_c.y()) >= fabs(v_c.x())) && (fabs(v_c.y()) >= fabs(v_c.z()))) {
        assert(fabs(v_c.y()) > DOUBLE_GAP);

        double k = v_p.y() / v_c.y();
        if((k < 0) || (k > 1)) return false;
        return true;
    }

    assert(fabs(v_c.z()) > DOUBLE_GAP);

    double k = v_p.z() / v_c.z();
    if((k < 0) || (k > 1)) return false;
    return true;
}

bool Intersection_Finder::check_intersection(const point &p1, const point &p2) {
    return is_points_match(p1, p2);
}


} //namespace geometry
