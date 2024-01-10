#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Image.h"

#include "Drawing.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int main(int argc, char **argv)
{
	/*
	Take in the following arguments in this order:

	- Output filename (ex: foo.png)
	- Image width (ex: 512)
	- Image height
	- Vertex 1 x-coord (int, in pixel coords)
	- Vertex 1 y-coord (...)
	- Vertex 2 x-coord
	- Vertex 2 y-coord
	- Vertex 3 x-coord
	- Vertex 3 y-coord
	*/
	if(argc < 9) {
		cout << "Usage: ./Lab1 <filename>.png width height vax vay vbx vby vcx vcy" << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);

	// Vertex 1 
	std::vector<int> v1({argv[4], argv[5]});
	std::vector<int> v2({argv[6], argv[7]});
	std::vector<int> v3({argv[8], argv[9]});

	// calculate max/min of the bounding box we need. 
	BoundingBox<int, 2> bb = BoundingBox<int, 2>();
	bb.add_vertex(v1); 
	bb.add_vertex(v2);
	bb.add_vertex(v3);
	
	

	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);
	// Draw a rectangle
	for(int y = 10; y < 20; ++y) {
		for(int x = 20; x < 40; ++x) {
			unsigned char r = 255;
			unsigned char g = 0;
			unsigned char b = 0;
			image->setPixel(x, y, r, g, b);
		}
	}
	// Write image to file
	image->writeToFile(filename);
	return 0;
}


