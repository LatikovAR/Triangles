#pragma once

#include <vector>
#include <ctime>
#include <cstdlib>

#include "geometry.h"

namespace geometry {

struct Rotatable_Object {
    Geometry_Object object;
    Cut axis;
    double speed;

    Rotatable_Object(Geometry_Object&& obj, Cut&& ax, double sp):
        object(obj), axis(ax), speed(sp) {}
};

class Rotator final {
private:
    std::vector<Rotatable_Object> objects_;
public:
    //speed should be in radians/seconds
    Rotator(std::vector<Rotatable_Object> objects);

    //time should be inputed in seconds
    //time = 0 - position of the base_objects_
    std::vector<Geometry_Object> cur_objects_pos(double time) const;

    size_t num_of_objects() const { return objects_.size(); }
};

} //namespace geometry
