
#include "Shape.h"
#include <iostream>
#include <cassert>

#include "GLSL.h"
#include "Program.h"

using namespace std;

Shape::Shape(bool textured) :
	eleBufID(0),
	posBufID(0),
	norBufID(0),
	texBufID(0), 
   vaoID(0)
{
	min = glm::vec3(0);
	max = glm::vec3(0);
	texOff = !textured;
}

Shape::~Shape()
{
}

// copy the data from the shape to this object
void Shape::createShape(tinyobj::shape_t & shape)
{
	posBuf = shape.mesh.positions;
	norBuf = shape.mesh.normals;
	texBuf = shape.mesh.texcoords;
	eleBuf = shape.mesh.indices;
}

void Shape::measure()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = std::numeric_limits<float>::max();
	maxX = maxY = maxZ = -std::numeric_limits<float>::max();

	//Go through all vertices to determine min and max of each dimension
	for (size_t v = 0; v < posBuf.size() / 3; v++)
	{
		if (posBuf[3*v+0] < minX) minX = posBuf[3 * v + 0];
		if (posBuf[3*v+0] > maxX) maxX = posBuf[3 * v + 0];

		if (posBuf[3*v+1] < minY) minY = posBuf[3 * v + 1];
		if (posBuf[3*v+1] > maxY) maxY = posBuf[3 * v + 1];

		if (posBuf[3*v+2] < minZ) minZ = posBuf[3 * v + 2];
		if (posBuf[3*v+2] > maxZ) maxZ = posBuf[3 * v + 2];
	}

	min.x = minX;
	min.y = minY;
	min.z = minZ;
	max.x = maxX;
	max.y = maxY;
	max.z = maxZ;
}

void Shape::init()
{
	// Initialize the vertex array object
	CHECKED_GL_CALL(glGenVertexArrays(1, &vaoID));
	CHECKED_GL_CALL(glBindVertexArray(vaoID));

	// Send the position array to the GPU
	CHECKED_GL_CALL(glGenBuffers(1, &posBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
	CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW));

	// Send the normal array to the GPU
	if (norBuf.empty())
	{
		// change this to generate the normals ourselves... 
		
		// norBufID = 0;

		// first, initialize all normal values to 0's (the normal buffer is the same size as the position buffer)
		for(size_t i = 0; i < this->posBuf.size(); ++i)
		{
			this->norBuf.push_back(0.0);
		}
		// for each facein our posBuf, we need to generate a normal for an associated vector
		for(size_t i = 0; i < this->eleBuf.size(); i += 3)
		{
			// here i1-3 are indices for the vertexes in posBuf we need to calculate normals for
			unsigned int i1 = this->eleBuf[i];
			unsigned int i2 = this->eleBuf[i+1];
			unsigned int i3 = this->eleBuf[i+2];

			// extract each vertex individually
			glm::vec3 v1 = glm::vec3(
				this->posBuf[3 * i1], 
				this->posBuf[3 * i1 + 1],
				this->posBuf[3 * i1 + 2]
			);
			glm::vec3 v2 = glm::vec3(
				this->posBuf[3 * i2], 
				this->posBuf[3 * i2 + 1],
				this->posBuf[3 * i2 + 2]
			);
			glm::vec3 v3 = glm::vec3(
				this->posBuf[3 * i3], 
				this->posBuf[3 * i3 + 1],
				this->posBuf[3 * i3 + 2]
			);

			// find difference vectors
			glm::vec3 e1 = v2 - v1;
			glm::vec3 e2 = v3 - v1;

			// calculate the unit normal
			glm::vec3 normal = glm::normalize(glm::cross(e2, e1));

			// add normal data for EACH vertex for our face. 
			// note we += here since we want to find the AVERAGE normal for all normals adjacent to some vertex

			// vertex 1
			this->norBuf[3*i1]    += normal.x;
			this->norBuf[3*i1 + 1] += normal.y;
			this->norBuf[3*i1 + 2] += normal.z;

			// vertex 2
			this->norBuf[3*i2]    += normal.x;
			this->norBuf[3*i2 + 1] += normal.y;
			this->norBuf[3*i2 + 2] += normal.z;

			// vertex 3
			this->norBuf[3*i3]    += normal.x;
			this->norBuf[3*i3 + 1] += normal.y;
			this->norBuf[3*i3 + 2] += normal.z;

		}

		// Lastly, we'll want to normalize ALL vectors in our buffer
		for(size_t i = 0; i < this->norBuf.size(); i += 3)
		{	
			float x = this->norBuf[i];
			float y = this->norBuf[i+1];
			float z = this->norBuf[i+2];

			glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));
			
			this->norBuf[i] = norm.x;
			this->norBuf[i+1] = norm.y;
			this->norBuf[i+2] = norm.z;
		}
	}
	
	// use normal data regardless and bind those buffers...
	CHECKED_GL_CALL(glGenBuffers(1, &norBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
	CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW));


	// Send the texture array to the GPU
	if (texBuf.empty())
	{
		texBufID = 0;
	}
	else
	{
		CHECKED_GL_CALL(glGenBuffers(1, &texBufID));
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
		CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW));
	}

	// Send the element array to the GPU
	CHECKED_GL_CALL(glGenBuffers(1, &eleBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));
	CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW));

	// Unbind the arrays
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Shape::draw(const shared_ptr<Program> prog) const
{
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

	CHECKED_GL_CALL(glBindVertexArray(vaoID));

	// Bind position buffer
	h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
	CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));

	// Bind normal buffer
	h_nor = prog->getAttribute("vertNor");
	if (h_nor != -1 && norBufID != 0)
	{
		GLSL::enableVertexAttribArray(h_nor);
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
		CHECKED_GL_CALL(glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
	}

	if (texBufID != 0)
	{
		// Bind texcoords buffer
		h_tex = prog->getAttribute("vertTex");

		if (h_tex != -1 && texBufID != 0)
		{
			GLSL::enableVertexAttribArray(h_tex);
			CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
			CHECKED_GL_CALL(glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0));
		}
	}

	// Bind element buffer
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));

	// Draw
	CHECKED_GL_CALL(glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0));

	// Disable and unbind
	if (h_tex != -1)
	{
		GLSL::disableVertexAttribArray(h_tex);
	}
	if (h_nor != -1)
	{
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}