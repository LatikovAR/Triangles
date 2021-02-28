#pragma once

#include <string>
#include <vector>

#include "geometry.h"
#include "rotator.h"

namespace geometry {

class Triangles_Generator final {
private:
    const size_t T_NUM = 1000;
    const double AREA_SIZE = 200.0;
    const double MAX_T_SIZE = 5.0;
    const int ACCURACY_COEF = 100;
    const double MAX_SPEED = 20.0;
    const int WORKTIME = 60; //in seconds

    double gen_number(int max_num) const;
    Rotatable_Object gen_rot_triangle(size_t number) const;
    void write_to_file(const std::vector<Rotatable_Object>& rot_trs,
                       const std::string& filename) const;
public:
    Triangles_Generator() = default;
    ~Triangles_Generator() = default;

    void generate(const std::string& filename) const;
};

} //namespace geometry
