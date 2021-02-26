#include <vector>
#include <ctime>
#include <stdexcept>
#include <cstdlib>

#include "geometry.h"
#include "rotator.h"

namespace geometry {

Rotator::Rotator(std::vector<Rotatable_Object>&& objects):
    objects_(objects)
{
    for(auto& rot_object : objects_) {
        rot_object.object.define_object();
    }
}

std::vector<Geometry_Object> Rotator::cur_objects_pos(double time) const {
    std::vector<Geometry_Object> rot_objects;
    rot_objects.reserve(objects_.size());

    for(size_t i = 0; i < objects_.size(); ++i) {
        const Rotatable_Object& cur_obj = objects_[i];
        double angle = time * 0.001 * cur_obj.speed;
        Geometry_Object new_cur_obj{cur_obj.object};
        new_cur_obj.rotate_object(cur_obj.axis, angle);
        rot_objects.push_back(new_cur_obj);
    }

    return rot_objects;
}

} //namespace geometry
