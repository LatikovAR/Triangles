#pragma once

#include <vector>
#include<stdexcept>

#include "Geometry/geometry.h"
#include "Geometry/rotator.h"
#include "Geometry/intersection_finder.h"

namespace vulkan {

class Draw_Triangles_Manager final {
private:
    const double POS_LIFETIME = 0.02; //in seconds

    geometry::Rotator rotator_; //objects inside
    const double lifetime_; //in seconds
public:
    Draw_Triangles_Manager(std::vector<geometry::Rotatable_Object> objects, double lifetime):
        rotator_(std::move(objects)), lifetime_(lifetime) {}

    void run();
};

class No_File_Exception final:
        std::runtime_error
{
public:
    using std::runtime_error::what;

    No_File_Exception(std::string information): std::runtime_error(information) {}
};

} //namespace vulkan
