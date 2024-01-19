#include "VecN.h"
#include "BoundingBox.h"

typedef struct 
{
    VecN<float, 3> v;
    Color c;
} VertexColorPair;

/**
 * A 3D triangle containing 3 vertices each with a color.  
*/
class Triangle
{
private:
    VertexColorPair v0;
    VertexColorPair v1;
    VertexColorPair v2;
    BoundingBox<float, 3> bb; 
public:
    Triangle(VertexColorPair v0, VertexColorPair v1, VertexColorPair v2) :
        v0(v0),
        v1(v1),
        v2(v2)
    {
        this->bb = BoundingBox<float,3>();
        this->bb.add_vertex(this->v0.v);
        this->bb.add_vertex(this->v1.v);
        this->bb.add_vertex(this->v2.v);
    };
    ~Triangle() {};

    BoundingBox<float, 3> getBoundingBox() { return this->bb; };
};


