#include <vector>
#include <ctime>
#include <stdexcept>
#include <cstdlib>

#include "geometry.h"
#include "rotator.h"

namespace geometry {

Rotator::Rotator(std::vector<Geometry_Object>&& objects,
        std::vector<Cut>&& axes,
        std::vector<double>&& speed):
    base_objects_(objects), axes_(axes), speed_(speed)
{
    if((base_objects_.size() != axes_.size()) ||
       (base_objects_.size() != speed_.size()))
        throw std::invalid_argument("wrong data sizes for rotation");

    for(auto& object : base_objects_) {
        object.define_object();
    }
}

std::vector<Geometry_Object> Rotator::cur_objects_pos(std::clock_t time) const {
    std::vector<Geometry_Object> rot_objects{base_objects_};

    for(size_t i = 0; i < num_of_objects(); ++i) {
        double angle = static_cast<double>(time) * 0.001 * speed_[i];
        rot_objects[i].rotate_object(axes_[i], angle);
    }

    return rot_objects;
}

} //namespace geometry
