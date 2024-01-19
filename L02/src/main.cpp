#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "Image.h"

#include "BoundingBox.h"
#include "VecN.h"

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

/**
 * Code to find a list of delimed substrings
 * taken from https://sentry.io/answers/split-string-in-cpp/
*/
template <typename T>
std::vector<T> getWords(std::string s, std::string delim)
{
	std::vector<T> res;
	unsigned int pos = 0;
	while(pos < s.size())
	{
		pos = s.find(delim);
		res.push_back(getAs<T>(s.substr(0,pos)));
		s.erase(0,pos+delim.size());
		pos = 0;
	}
	return res;
}

/**
 * Get nD Vertices a total of v_num number of times for some arbitrary type. 
 * You must input a comma-separated list of the appropriate T type a total of N times to get the 
*/
template <typename T, int N>
std::vector<VecN<T, N>> getVertex(unsigned int v_num)
{

	std::vector<VecN<T, N>> data;

	std::cout << "Getting input for " << std::to_string(v_num) << " vertices..." << std::endl;
	for(unsigned int i = 0; i < v_num; i++)
	{
		std::string in;
		std::cout << "Input new Vertex (ex: \"T1, ..., (up to) T" << std::to_string(N) << "\" ): Currently on Vertex # " << std::to_string(i) << std::endl;
		std::getline(std::cin, in);
		// create a new vector from the list we made and push it to our data vector.
		data.push_back(VecN<T,N>(getWords<T>(in, ",")));

	}
	return data;
}

int main(int argc, char **argv)
{
	/**
	 * Read in 3 vertices from stdin to represent our triangle
	 * Assign different color values to each vertex. 
	 * Compute the bounding box of the triangle using L01
	 * Computer the barycentric coordinates for every pixel in the bounding box
	 * For each pixel within the triangle:
	 *  - color the pixel using the interpolated color
	 * Write out the pixel values as an image (from last time)
	*/

	// 0. Get the width and height as limiting factors...
	std::string in; // string input
	unsigned int width; 
	unsigned int height;

	std::cout << "What is the width of your image?" << std::endl;
	std::getline(std::cin, in);
	width = getAs<unsigned int>(in);
	std::cout << "What is the height of your image?" << std::endl;
	std::getline(std::cin, in);
	height = getAs<unsigned int>(in);

	auto image = std::make_shared<Image>(width, height);
	const Color background = BLACK;

	// 1. Get the data of each vertex 
	auto vs = getVertex<int, 2>(3);
	/* For debugging, print out the following information... */
	std::cout << "You entered the following vertices: " << std::endl;
	for(auto elem : vs)
	{
		std::cout << elem << std::endl;
	}
	/* Note that now that we have the vertices, we can calculate the area of the total triangle for later */
	double A = calc_TriArea(vs[0], vs[1], vs[2]);


	// 2. Assign different color values to each vertex (predefined)
	std::vector<Color> cs;
	std::cout << "Now asking for 3 colors for your vertices..." << std::endl;
	std::cout << "Please enter in any combinations of your three colors. For example, enter \"r + g\" to get RED + GREEN" << std::endl;
	std::cout << "You can enter in any colors from the following: (r: RED, g: GREEN, b: BLUE, y: YELLOW, o: ORANGE, p: PURPLE)" << std::endl;
	for(unsigned int i = 0; i < 3; i++)
	{
		
		Color temp;
		std::string in;

		std::cout << "Asking for Vertex " << std::to_string(i) << ":" << std::endl;
		std::getline(std::cin, in);
		std::vector<std::string> chosen_colors = getWords<std::string>(in, "+");
		temp = BLACK;
		for(std::string elem : chosen_colors)
		{
			if(elem.find("r") != elem.npos)
				temp += RED;
			if(elem.find("g") != elem.npos)
				temp += GREEN;
			if(elem.find("b") != elem.npos)
				temp += BLUE;
			if(elem.find("y") != elem.npos)
				temp += YELLOW;
			if(elem.find("o") != elem.npos)
				temp += ORANGE;
			if(elem.find("p") != elem.npos)
				temp += PURPLE;
		}
		cs.push_back(temp);
	}


	// 3. Compute the bounding box of the triangle using L01 (remember that points are pixel coordinates)
	BoundingBox<int, 2> bb = BoundingBox<int, 2>();
	bb.add_vertex(vs[0]);
	bb.add_vertex(vs[1]);
	bb.add_vertex(vs[2]);
	/* Now bb has the max and min points of our box, so just take combinations of points on the inside... */
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			// we iterate through all possible points here...
			VecN<int, 2> test_pt = VecN<int, 2>({x, y});
			if(bb.in_box(test_pt))
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
					Color new_pixel_color = BLACK;
					new_pixel_color += (cs[0] * alpha);
					new_pixel_color += (cs[1] * beta);
					new_pixel_color += (cs[2] * gamma);
					image->setPixel(
						x, 
						y, 
						new_pixel_color[0], 
						new_pixel_color[1], 
						new_pixel_color[2]
					);
				}
				else // write background pixel
				{
					image->setPixel(
						x,
						y,
						background[0],
						background[1], 
						background[2]
					);
				}
			
			}
			else // write background pixel
			{
				image->setPixel(
						x,
						y,
						background[0],
						background[1], 
						background[2]
				);
			}
		}
	}

	// Very lastly, write to our output!!!
	image->writeToFile("output.png");
}


