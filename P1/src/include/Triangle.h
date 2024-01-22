#include "VecN.h"
#include "BoundingBox.h"

/**
 * Vertex-Color pair in PIXEL coordinates...
 * We expect pixel coordinate vectors + colors for each...
*/
template <typename T, int N>
struct VertexColorPair
{
    VecN<T, N> v;
    Color c;
};

/**
 * A 3D triangle containing 3 vertices each with a color.  
*/
template <typename T, int N>
class Triangle
{
private:
    VertexColorPair<T,N> v0;
    VertexColorPair<T,N> v1;
    VertexColorPair<T,N> v2;
    BoundingBox<T, N> bb; 
public:
    Triangle(VertexColorPair<T,N> v0, VertexColorPair<T,N> v1, VertexColorPair<T,N> v2) :
        v0(v0),
        v1(v1),
        v2(v2)
    {
        this->bb = BoundingBox<T,N>();
        this->bb.add_vertex(this->v0.v);
        this->bb.add_vertex(this->v1.v);
        this->bb.add_vertex(this->v2.v);
    };
    ~Triangle() {};

    BoundingBox<T, N> getBoundingBox() { return this->bb; };
};


