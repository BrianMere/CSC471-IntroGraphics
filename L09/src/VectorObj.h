#ifndef MY_VECTOROBJ_H
#define MY_VECTOROBJ_H

#include "function_generator.h"
#include "Bobject.h"

#include <string>
#include <glm/glm.hpp>

#define CONE_FILENAME "cone.obj"
#define CYLINDER_HALF_FN "cylinder_half.obj"
#define SUB_DIR "/math"

using namespace glm;

/**
 * My own implementation of a vector object in openGL.
 * 
 * This object contains the cylinder part. 
 * The other hiearchial parts are the cylinder part. 
*/
class VectorObj : public Bobject<>
{
public:
    VectorObj(const std::string& resourceDir, std::shared_ptr<Program> prog)
    : Bobject(resourceDir, prog)
    {};
    ~VectorObj() {};

    void init() override;
};

#endif
