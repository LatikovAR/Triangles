#pragma once

#include <vector>
#include <utility>

#include "geometry.h"
#include "rotator.h"
#include "intersection_finder.h"

namespace vulkan {

class Draw_Triangles_Manager final {
private:
    const unsigned long long POS_LIFETIME = 50; //in milliseconds

    geometry::Rotator rotator_; //objects inside
public:
    Draw_Triangles_Manager(std::vector<geometry::Rotatable_Object>&& objects):
        rotator_(std::move(objects)) {}

    void run();
};

} //namespace vulkan
