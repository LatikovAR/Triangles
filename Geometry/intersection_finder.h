#pragma once

#include <cstdlib>
#include <vector>
#include <stdexcept>

#include "geometry.h"

namespace geometry {


//-----------------------------------Objects_and_Intersections--------------------

class Objects_and_Intersections final {
private:
    std::vector<Geometry_Object> objects_;
    std::vector<bool> intersection_flags_;
public:
    //Flags order is object numbers order
    Objects_and_Intersections(std::vector<Geometry_Object> objects,
                              std::vector<bool> intersection_flags):
        objects_(std::move(objects)),
        intersection_flags_(std::move(intersection_flags))
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



//---------------------------------Intersection_finder----------------------------


class Intersection_Finder final {
private:
    std::vector<Geometry_Object> objects_;
    std::vector <bool> intersection_flags_;

    //this methods for computing intersections algorithm
    void compute_intersections_recursive_algorithm(
            const std::vector<Geometry_Object*>& p_objects);

    void root_triangle_case(const std::vector<Geometry_Object*>& p_objects);
    void root_cut_case(const std::vector<Geometry_Object*>& p_objects);
    void root_point_case(const std::vector<Geometry_Object*>& p_objects);

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
public:
    Intersection_Finder(std::vector<Geometry_Object> objects);

    Objects_and_Intersections compute_intersections();
};

} //namespace geometry
