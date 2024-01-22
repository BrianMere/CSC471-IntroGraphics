/* Release code for program 1 CPE 471 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
#include <sstream>
#include <limits>

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

   //create an image, and our z-buffer with values of our maximum depth
	auto image = make_shared<Image>(g_width, g_height);
   double Zbuff[g_width][g_height];
   for(unsigned int i = 0; i < g_width; i++)
   {
      for(unsigned int j = 0; j < g_height; j++)
      {
         Zbuff[i][j] = -std::numeric_limits<double>::max();
      }
   }
      


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
   for(size_t i = 0; i < triBuf.size()/3; i += 3)
   {
      // 1. Get the current triangle we are on...
      std::vector<unsigned int> choose_vecs = GETVEC3(unsigned int, triBuf, i);

      // use the obj to search for the vertices in the posBuf to construct our triangle.
      VertexColorPair<float, 3> v1 = {.v = VecN<float, 3>(GETVEC3(float, posBuf, choose_vecs[0])), .c = RED};
      VertexColorPair<float, 3> v2 = {.v = VecN<float, 3>(GETVEC3(float, posBuf, choose_vecs[1])), .c = RED};
      VertexColorPair<float, 3> v3 = {.v = VecN<float, 3>(GETVEC3(float, posBuf, choose_vecs[2])), .c = RED};

      // 2. Construct and interpret our triangle to possibly write pixels in its bounding box...
      // ... convert our triangle vector into pixel coordinates for our L02 algorithm ... 
      std::vector<VecN<int, 2>> vs = std::vector<VecN<int,2>>();
      vs.push_back(VecN<int,2>({
         (int) wtpc.W2PX(v1.v[0]),
         (int) wtpc.W2PY(v1.v[1])
      }));
      vs.push_back(VecN<int,2>({
         (int) wtpc.W2PX(v2.v[0]),
         (int) wtpc.W2PY(v2.v[1])
      }));
      vs.push_back(VecN<int,2>({
         (int) wtpc.W2PX(v3.v[0]),
         (int) wtpc.W2PY(v3.v[1])
      }));
      double A = calc_TriArea(vs[0], vs[1], vs[2]);

      Triangle<int, 2> tri = Triangle<int, 2>(
         VertexColorPair<int, 2>({.v = vs[0], .c = v1.c}),
         VertexColorPair<int, 2>({.v = vs[1], .c = v2.c}),
         VertexColorPair<int, 2>({.v = vs[2], .c = v3.c})
      );
      
      unsigned int xmin = tri.getBoundingBox().getMin(0); // x-min
      unsigned int ymin = tri.getBoundingBox().getMin(1); // y-min
      unsigned int xmax = tri.getBoundingBox().getMax(0); // x-max
      unsigned int ymax = tri.getBoundingBox().getMax(1); // y-max

      // for each pixel in the bounding box of this triangle...
      for(int x = xmin; x < xmax; x++)
      {
         for(int y = ymin; y < ymax; y++)
         {
            // 3. Compute Barycentric Coordinates as we did in L02...
            VecN<int, 2> test_pt = VecN<int, 2>({x, y, 0});

            if(tri.getBoundingBox().in_box(test_pt))
			   {
               // 4. Compute the Barycentric coordinates for each point here
               /* We already have A, we need to find the smaller areas for the subtriangles for each alpha, beta, gamma */
               // for alpha, we exclude considering v0, for beta exclude v1, ... and substitute with test_pt
               double alpha = calc_TriArea(test_pt, vs[1], vs[2]) / A;
               double beta = calc_TriArea(test_pt, vs[2], vs[0]) / A;
               double gamma = calc_TriArea(test_pt, vs[0], vs[1]) / A;
            

               // 5. Using the Barycentric Calculations prior, determine if we are in the triangle (ie: all values in (0,1) range)
               // - if we are, color based on the alpha, beta, gamma values from (4), otherwise, color in as background...
               if(	alpha >= 0 && alpha <= 1 &&
                  beta >= 0 && beta <= 1 &&
                  gamma >= 0 && gamma <= 1) // write our determined triangle color...
               {

                  // 6. Do a z-buffer test, and add if we need to...
                  double current_z = (double)(alpha*v1.v[2] + beta*v2.v[2] + gamma*v3.v[2]);
                  Color current_col = BLACK;
                  current_col += (v1.c * alpha);
                  current_col += (v2.c * beta);
                  current_col += (v3.c * gamma);
                  if(Zbuff[x][y] < current_z)
                  {
                     // color pixel, update zbuf
                     Zbuff[x][y] = current_z;
                     image->setPixel(
                        x, 
                        y, 
                        current_col[0], 
                        current_col[1], 
                        current_col[2]
                     );
                  }
               }
            }
         }
      }
   }

   /*******************************************************************/
   /* MY CODE ENDS HERE */
	
	//write out the image
   image->writeToFile(imgName);

	return 0;
}
