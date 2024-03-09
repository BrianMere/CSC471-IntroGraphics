#pragma once
#ifndef __particleS__
#define __particleS__

#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include "Particle.h"
#include "Program.h"

using namespace glm;
using namespace std;
using force_t = glm::vec3(*)(glm::vec3, Particle);
using dist_t = glm::vec3(*)(float);

class ParticleSorter {
public:
   bool operator()(const shared_ptr<Particle> p0, const shared_ptr<Particle> p1) const
   {
      // Particle positions in world space
      const vec3 &x0 = p0->getPosition();
      const vec3 &x1 = p1->getPosition();
      // Particle positions in camera space
      vec4 x0w = C * vec4(x0.x, x0.y, x0.z, 1.0f);
      vec4 x1w = C * vec4(x1.x, x1.y, x1.z, 1.0f);
      return x0w.z < x1w.z;
   }
  
   mat4 C; // current camera matrix
};

class particleSys {
private:
	vector<shared_ptr<Particle>> particles;

	force_t F; //force function
	dist_t dist; // distribution function (of particles)
	dist_t colorDist; // distribution function for color

	float t, h; //?
	int numP;
	vec3 start;
	ParticleSorter sorter;
	//this is not great that this is hard coded - you can make it better
	GLfloat points[900];
	GLfloat pointColors[1200];

	mat4 theCamera;
	unsigned vertArrObj;
	unsigned vertBuffObj;

	unsigned colorArrObj;
	unsigned colorBuffObj;
	
public:

	particleSys();
	void drawMe(std::shared_ptr<Program> prog);
	void gpuSetup();
	void update();
	void reSet();
	void setCamera(mat4 inC) {theCamera = inC;}

	/**
	 * Sets the origin in world space of our particle system, defining the vector field relative to this point.
	*/
	void setOrigin(glm::vec3 source);

	/**
	 * Sets the force function based on position and velocity (and other characteristics) of our particle
	*/
	void setForce(force_t Force);

	/**
	 * Sets the function of distribution of points, on a 1-dimensional line. This distribution will select 
	 * particle position at times t from [0,1] inclusive
	*/
	void setDist(dist_t Distribution);

	/**
	 * Sets the color distribution function.
	*/
	void setColorDist(dist_t color_Dist);
};


#endif
