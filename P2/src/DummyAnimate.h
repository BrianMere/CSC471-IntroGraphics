#include "Object.h"

class DummyAnimate : public Object 
{
private:
    /* data */
public:
    DummyAnimate(const std::string & resourceDir)
    : Object(resourceDir, "dummy.obj") {};
    ~DummyAnimate() {};
};


