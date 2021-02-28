#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <cassert>

#include "triangles_generator.h"

namespace geometry {

void Triangles_Generator::generate(const std::string& filename) const {
    std::srand(std::time(0));

    std::vector<Rotatable_Object> rot_trs;
    rot_trs.reserve(T_NUM);
    for(size_t i = 0; i < T_NUM; ++i) {
        rot_trs.push_back(gen_rot_triangle(i));
    }

    write_to_file(rot_trs, filename);
}

Rotatable_Object Triangles_Generator::gen_rot_triangle(size_t number) const {
    double x, y, z;
    x = gen_number(AREA_SIZE);
    y = gen_number(AREA_SIZE);
    z = gen_number(AREA_SIZE);
    vec shift{x, y, z};

    x = gen_number(MAX_T_SIZE);
    y = gen_number(MAX_T_SIZE);
    z = gen_number(MAX_T_SIZE);
    point p1 = point{x, y, z} + shift;

    x = gen_number(MAX_T_SIZE);
    y = gen_number(MAX_T_SIZE);
    z = gen_number(MAX_T_SIZE);
    point p2 = point{x, y, z} + shift;

    while (is_points_match(p1, p2)) {
        x = gen_number(MAX_T_SIZE);
        y = gen_number(MAX_T_SIZE);
        z = gen_number(MAX_T_SIZE);
        p2 = point{x, y, z} + shift;
    }

    x = gen_number(MAX_T_SIZE);
    y = gen_number(MAX_T_SIZE);
    z = gen_number(MAX_T_SIZE);
    point p3 = point{x, y, z} + shift;

    while((is_points_match(p1, p3)) ||
          (is_points_match(p2, p3)) ||
          (is_points_on_one_line(p1, p2, p3)))
    {
        x = gen_number(MAX_T_SIZE);
        y = gen_number(MAX_T_SIZE);
        z = gen_number(MAX_T_SIZE);
        p3 = point{x, y, z} + shift;
    }

    Triangle t{p1, p2, p3};


    x = gen_number(MAX_T_SIZE);
    y = gen_number(MAX_T_SIZE);
    z = gen_number(MAX_T_SIZE);
    p1 = point{x, y, z} + shift;

    x = gen_number(MAX_T_SIZE);
    y = gen_number(MAX_T_SIZE);
    z = gen_number(MAX_T_SIZE);
    p2 = point{x, y, z} + shift;

    while (is_points_match(p1, p2)) {
        x = gen_number(MAX_T_SIZE);
        y = gen_number(MAX_T_SIZE);
        z = gen_number(MAX_T_SIZE);
        p2 = point{x, y, z} + shift;
    }

    Cut axis{p1, p2};

    double speed = gen_number(MAX_SPEED);

    return Rotatable_Object{Geometry_Object{number, t}, axis, speed};
}

double Triangles_Generator::gen_number(int max_num) const {
    return static_cast<double>(std::rand() % (max_num * ACCURACY_COEF)) /
           static_cast<double>(ACCURACY_COEF);
}

void Triangles_Generator::write_to_file(const std::vector<Rotatable_Object> &rot_trs,
                                        const std::string& filename) const
{
    std::ofstream out;
    out.open(filename);

    out << T_NUM << std::endl;
    out << WORKTIME << std::endl;

    for(const Rotatable_Object& obj : rot_trs) {
        assert(obj.object.type() == TRIANGLE);
        const Triangle& t = obj.object.triangle();
        out << t.p1().x() << " " << t.p1().y() << " " << t.p1().z() << " ";
        out << t.p2().x() << " " << t.p2().y() << " " << t.p2().z() << " ";
        out << t.p3().x() << " " << t.p3().y() << " " << t.p3().z() << std::endl;

        point p = obj.axis.p_begin();
        out << p.x() << " " << p.y() << " " << p.z() << " ";
        p = obj.axis.p_end();
        out << p.x() << " " << p.y() << " " << p.z() << std::endl;

        out << obj.speed << std::endl;
    }
}

} //namespace geometry
