#include "Object.h"
#include "Program.h"
#include "function_generator.h"

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
class VectorObj
{
private:    
    std::string SetupObjs(const std::string&);
    std::map<std::string, std::shared_ptr<Object>> myContainer;
public:
    std::shared_ptr<Object> parentObj; // access this object from this container for obj purposes. 

    VectorObj(const std::string& resourceDir, std::shared_ptr<Program> prog)
    {
        this->myContainer["Cone"] = std::make_shared<Object>(
            resourceDir + SUB_DIR, 
            CONE_FILENAME, 
            prog
        );

        this->myContainer["CylinderTH"] = std::make_shared<Object>(
            resourceDir + SUB_DIR, 
            CYLINDER_HALF_FN, 
            prog
        );

        this->myContainer["CylinderBH"] = std::make_shared<Object>(
            resourceDir + SUB_DIR, 
            CYLINDER_HALF_FN, 
            prog
        );

        // rotate BH to actually be BH, then move cone to center and translate by 1. 
        // by default points one unit in the x-dir
        this->myContainer["CylinderBH"]->add_transform(rotate(mat4(1.0f), M_PIf, vec3(1,0,0)));
        this->myContainer["Cone"]->add_transform(translate(mat4(1.0f), vec3(1,0,0)));

        // lastly add these objects under the parent Guide
        this->myContainer["Cone"]->add_subobj(this->myContainer["CylinderTH"]);
        this->myContainer["Cone"]->add_subobj(this->myContainer["CylinderBH"]);

        this->parentObj = this->myContainer["Cone"];
    };
    ~VectorObj() {};
};

std::string VectorObj::SetupObjs(const std::string & resourceDirectory)
{
    funcToObj(
        resourceDirectory + "/math",
        CYLINDER_HALF_FN,
        100, 
        [](float x, float y) { return sqrt(1 - 0.5 * pow(y, 2)); },
        -1.0, 
        1.0, 
        -1.0, 
        1.0
    );

    funcToObj(
        resourceDirectory + "/math",
        CONE_FILENAME,
        100, 
        [](float x, float y) { return sqrt(pow(x,2) + pow(y, 2)); },
        -1.0, 
        1.0, 
        -1.0, 
        1.0
    );

    return resourceDirectory;
}
