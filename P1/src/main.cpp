/* Release code for program 1 CPE 471 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
#include <sstream>

#include "tiny_obj_loader.h"
#include "Image.h"
#include "VecN.h"
#include "Triangle.h"
#include "WorldToPixelConverter.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

/*
   Helper function you will want all quarter
   Given a vector of shapes which has already been read from an obj file
   resize all vertices to the range [-1, 1]
 */
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   float scaleX, scaleY, scaleZ;
   float shiftX, shiftY, shiftZ;
   float epsilon = 0.001;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

         if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

         if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
      }
   }

	//From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
      maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
      maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
      maxExtent = zExtent;
   }
   scaleX = 2.0 /maxExtent;
   shiftX = minX + (xExtent/ 2.0);
   scaleY = 2.0 / maxExtent;
   shiftY = minY + (yExtent / 2.0);
   scaleZ = 2.0/ maxExtent;
   shiftZ = minZ + (zExtent)/2.0;

   //Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
         assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
      }
   }
}


/**
 * Turn a string into some generic passed type
 * (taken from https://stackoverflow.com/questions/69238675/how-to-generically-convert-a-string-to-another-type-using-a-template-function)
 *
*/
template <typename T>
T getAs(const std::string& s)
{
	std::stringstream ss{s};
	T t;
	ss >> t;
	return t;
}

int main(int argc, char **argv)
{
	if(argc < 6) {
      cout << "Usage: ./path/to/P1 meshfilelocation.obj imagefilename.png img_width img_height coloring_mode" << endl;
      return 0;
   }
	// OBJ filename
	string meshName(argv[1]);
	string imgName(argv[2]);

	//set g_width and g_height appropriately!
	unsigned int g_width = getAs<unsigned int>(std::string(argv[3]));
   unsigned int g_height = getAs<unsigned int>(std::string(argv[4]));
   int coloring_mode = getAs<int>(std::string(argv[5]));

   //create an image
	auto image = make_shared<Image>(g_width, g_height);

	// triangle buffer
	vector<unsigned int> triBuf;
	// position buffer
	vector<float> posBuf;
   #define GETVEC3(T,arr,i) (std::vector<T>({arr[3*i], arr[3*i + 1], arr[3*i + 2]}))


	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	vector<tinyobj::shape_t> shapes; // geometry
	vector<tinyobj::material_t> objMaterials; // material
	string errStr;
	
   bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());
	/* error checking on read */
	if(!rc) {
		cerr << errStr << endl;
	} else {
 		//keep this code to resize your object to be within -1 -> 1
   	resize_obj(shapes); 
		posBuf = shapes[0].mesh.positions;
		triBuf = shapes[0].mesh.indices;
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	cout << "Number of triangles: " << triBuf.size()/3 << endl;

   /* MY CODE STARTS HERE */
   /*******************************************************************/

   WorldToPixelConverter wtpc = WorldToPixelConverter(g_width,g_height);
   for(size_t i = 0; i < triBuf.size(); i += 3)
   {
      // 1. Get the current triangle we are on...
      std::vector<unsigned int> choose_vecs = GETVEC3(unsigned int, triBuf, i);

      // use the obj to search for the vertices in the posBuf to construct our triangle.
      VertexColorPair v1 = {.v = VecN<float, 3>(GETVEC3(float, posBuf, choose_vecs[0])), .c = RED};
      VertexColorPair v2 = {.v = VecN<float, 3>(GETVEC3(float, posBuf, choose_vecs[1])), .c = GREEN};
      VertexColorPair v3 = {.v = VecN<float, 3>(GETVEC3(float, posBuf, choose_vecs[2])), .c = BLUE};

      // 2. Construct and interpret our triangle to possibly write pixels in its bounding box...
      Triangle tri = Triangle(v1, v2, v3);
      unsigned int xmin = wtpc.W2PX(tri.getBoundingBox().getMin(0)); // x-min
      unsigned int ymin = wtpc.W2PY(tri.getBoundingBox().getMin(1)); // y-min
      unsigned int xmax = wtpc.W2PX(tri.getBoundingBox().getMax(0)); // x-max
      unsigned int ymax = wtpc.W2PY(tri.getBoundingBox().getMax(1)); // y-max

      // for each pixel in the bounding box of this triangle...
      for(int x = xmin; x < xmax; x++)
      {
         for(int y = ymin; y < ymax; y++)
         {
            // 3. Compute Barycentric Coordinates as we did in L02...
            VecN<int, 2> test_pt = VecN<int, 2>({x, y}); 
         }
      }
   }

   /*******************************************************************/
   /* MY CODE ENDS HERE */
	
	//write out the image
   image->writeToFile(imgName);

	return 0;
}
