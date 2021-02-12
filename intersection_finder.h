#pragma once

#include <cstdlib>
#include <vector>
#include <stdexcept>

#include "geometry.h"

namespace geometry {

enum g_obj_type {TRIANGLE, CUT, POINT, UNDEFINED};

//------------------------------------Geometry_Objects------------------------------

class Undefined_Object final {
private:
    point p1_;
    point p2_;
    point p3_;
public:
    const point& p1() const { return p1_; }
    const point& p2() const { return p2_; }
    const point& p3() const { return p3_; }

    Undefined_Object(const point &p1, const point &p2, const point &p3):
        p1_(p1), p2_(p2), p3_(p3) {}
};

class Geometry_Object final {
private:
    size_t number_;
    g_obj_type obj_type_;
    union {
        point p_;
        Cut c_;
        Triangle t_;
        Undefined_Object u_o_;
    };
public:
    size_t number() const { return number_; }
    g_obj_type type() const { return obj_type_; }

    const point& pointt() const { return p_; }
    const Cut& cut() const { return c_; }
    const Triangle& triangle() const { return t_; }

    Geometry_Object() {}

    Geometry_Object(size_t number, const Undefined_Object& undef_obj):
        number_(number), obj_type_(UNDEFINED), u_o_(undef_obj) {}
    //virtual ~Geometry_Object() {}

    void define_object();

    static bool check_intersection(const Triangle &t, const Triangle &t1);
    static bool check_intersection(const Triangle &t, const Cut &c);
    static bool check_intersection(const Cut &c, const Triangle &t) {
        return check_intersection(t, c);
    }
    static bool check_intersection(const Triangle &t, const point &p);
    static bool check_intersection(const point &p, const Triangle &t) {
        return check_intersection(t, p);
    }
    static bool check_intersection(const Cut &c1, const Cut &c2);
    static bool check_intersection(const Cut &c, const point &p);
    static bool check_intersection(const point &p, const Cut &c) {
        return check_intersection(c, p);
    }
    static bool check_intersection(const point &p1, const point &p2);
};

/*
class Object_Point final :
        public Geometry_Object,
        public point {
public:
    Object_Point(const point& p, size_t num):
        Geometry_Object(num), point(p){}
};

class Object_Cut final :
        public Geometry_Object,
        public Cut {
public:
    Object_Cut(const Cut& c, size_t num):
        Geometry_Object(num), Cut(c){}
};

class Object_Triangle final :
        public Geometry_Object,
        public Triangle {
public:
    Object_Triangle(const Triangle& t, size_t num):
        Geometry_Object(num), Triangle(t) {}
};*/




//------------------------------Geometry_Objects_Storage---------------------------
/*
class Geometry_Object_Storage final {
private:
    std::vector<Object_Point> obj_point_storage_;
    std::vector<Object_Cut> obj_cut_storage_;
    std::vector<Object_Triangle> obj_triangle_storage_;
public:
    std::vector<Object_Point>& points() { return obj_point_storage_; }
    std::vector<Object_Cut>& cuts() { return obj_cut_storage_; }
    std::vector<Object_Triangle>& triangles() { return obj_triangle_storage_; }

    const std::vector<Object_Point>& points() const { return obj_point_storage_; }
    const std::vector<Object_Cut>& cuts() const { return obj_cut_storage_; }
    const std::vector<Object_Triangle>& triangles() const { return obj_triangle_storage_; }

    Geometry_Object_Storage(const std::vector<Undefined_Object>& undef_objects);

    size_t capacity() const { return obj_point_storage_.size() +
                                     obj_cut_storage_.size() +
                                     obj_triangle_storage_.size(); }
};
*/



//---------------------------------Intersection_finder----------------------------

class Objects_and_Intersections final {
private:
    std::vector<Geometry_Object> objects_;
    std::vector<bool> intersection_flags_;
public:
    //Flags order is object numbers order
    Objects_and_Intersections(std::vector<Geometry_Object>&& objects,
                              std::vector<bool>&& intersection_flags):
        objects_(objects),
        intersection_flags_(intersection_flags)
    {
        if(objects.size() != intersection_flags.size())
            throw std::invalid_argument("flags array isn't compatible with objects storage");
    }

    size_t objects_num() const { return objects_.size(); }

    const std::vector<Geometry_Object>& objects() const { return objects_; }
    const std::vector<bool>& intersection_flags() const {
        return intersection_flags_;
    }

    const Geometry_Object& object(size_t num) const {
        if(num >= objects_num()) throw std::invalid_argument("invalid object num");
        return objects_[num];
    }
    bool is_object_intersects(size_t num) const {
        if(num >= objects_num()) throw std::invalid_argument("invalid object num");
        return intersection_flags_[num];
    }
};

class Intersection_Finder final {
private:
    std::vector<Geometry_Object> objects_;
    std::vector <bool> intersection_flags_;
    size_t iter1 = 0, iter2 = 0, iter3 = 0, iter4 = 0;

    //this methods for computing intersections algorithm
    void compute_intersections_recursive_algorithm(
            const std::vector<Geometry_Object*>& p_objects);

    void root_triangle_case(const std::vector<Geometry_Object*>& p_objects);
    void root_cut_case(const std::vector<Geometry_Object*>& p_objects);
    void root_point_case(const std::vector<Geometry_Object*>& p_objects);
public:
    Intersection_Finder(std::vector<Geometry_Object>&& objects);

    Objects_and_Intersections compute_intersections();
};

} //namespace geometry
