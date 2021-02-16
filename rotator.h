#pragma once

#include <vector>
#include <ctime>
#include <cstdlib>

#include "geometry.h"

namespace geometry {

class Rotator final {
private:
    std::vector<Geometry_Object> base_objects_;
    std::vector<Cut> axes_;
    std::vector<double> speed_;
public:
    //speed should be in radians/seconds
    Rotator(std::vector<Geometry_Object>&& objects,
            std::vector<Cut>&& axes, std::vector<double>&& speed);

    //time should be inputed in milliseconds
    //time = 0 - position of the base_objects_
    std::vector<Geometry_Object> cur_objects_pos(std::clock_t time) const;

    size_t num_of_objects() const { return base_objects_.size(); }
};

} //namespace geometry
