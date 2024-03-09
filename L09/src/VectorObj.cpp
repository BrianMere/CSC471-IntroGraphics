#include "VectorObj.h"

void VectorObj::init()
{
    funcToObj(
        this->resourceDir + SUB_DIR,
        CYLINDER_HALF_FN,
        100, 
        [](float x, float y) { return sqrt(1 - 0.5 * pow(y, 2)); },
        -1.0, 
        1.0, 
        -1.0, 
        1.0
    );

    funcToObj(
        this->resourceDir + SUB_DIR,
        CONE_FILENAME,
        100, 
        [](float x, float y) { return sqrt(pow(x,2) + pow(y, 2)); },
        -1.0, 
        1.0, 
        -1.0, 
        1.0
    );

    this->objs["Cone"] = std::make_shared<Object>(
        this->resourceDir + SUB_DIR, 
        CONE_FILENAME, 
        this->prog
    );

    this->objs["CylinderTH"] = std::make_shared<Object>(
        this->resourceDir + SUB_DIR, 
        CYLINDER_HALF_FN, 
        this->prog
    );

    this->objs["CylinderBH"] = std::make_shared<Object>(
        this->resourceDir + SUB_DIR, 
        CYLINDER_HALF_FN, 
        this->prog
    );

    // rotate BH to actually be BH, then move cone to center and translate by 1. 
    // by default points one unit in the x-dir
    this->objs["CylinderBH"]->add_transform(rotate(mat4(1.0f), M_PIf, vec3(1,0,0)));
    this->objs["Cone"]->add_transform(translate(mat4(1.0f), vec3(1,0,0)));
}
