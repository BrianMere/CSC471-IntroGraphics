#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "particleSys.h"
#include "Particle.h"
#include "GLSL.h"

using namespace std;

particleSys::particleSys() {

	numP = 300;	
	t = 0.0f;
	h = 0.01f;
	F = 0;
  dist = 0;
  colorDist = [](float t){ return glm::vec3(1.0, 1.0, 1.0); };
	theCamera = glm::mat4(1.0);
}

void particleSys::setForce(force_t Force)
{
  F = Force;
}

void particleSys::gpuSetup() {

	for (int i=0; i < numP; i++) {
    glm::vec3 startingPoint = this->start + this->dist(randFloat(0.0f, 1.0f));
    auto particle = make_shared<Particle>(startingPoint);
		glm::vec3 startingColor = this->colorDist(randFloat(0.0f, 1.0f));

    particle->setStartingVelocity(F(startingPoint, *particle));
    particle->setColor(startingColor);

    points[i*3+0] = startingPoint.x;
		points[i*3+1] = startingPoint.y;
		points[i*3+2] = startingPoint.z;

    glm::vec4 c = particle->getColor();

    pointColors[i*4 + 0] = c.r;
    pointColors[i*4 + 1] = c.g;
    pointColors[i*4 + 2] = c.b;
    pointColors[i*4 + 3] = c.a;
    
		particles.push_back(particle);
		particle->load();
	}

	//generate the VAO
   CHECKED_GL_CALL(glGenVertexArrays(1, &vertArrObj));
   CHECKED_GL_CALL(glBindVertexArray(vertArrObj));

   CHECKED_GL_CALL(glGenVertexArrays(1, &colorArrObj));
   CHECKED_GL_CALL(glBindVertexArray(colorArrObj));

   //generate vertex buffer to hand off to OGL - using instancing
   CHECKED_GL_CALL(glGenBuffers(1, &vertBuffObj));
   //set the current state to focus on our vertex buffer
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj));
   //actually memcopy the data - only do this once
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STREAM_DRAW));

   CHECKED_GL_CALL(glGenBuffers(1, &colorBuffObj));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorBuffObj));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), &pointColors[0], GL_STREAM_DRAW));
   
   assert(glGetError() == GL_NO_ERROR);
	
}

void particleSys::reSet() {
	for (int i=0; i < numP; i++) {
		particles[i]->load();
	}
}

void particleSys::drawMe(std::shared_ptr<Program> prog) {

  glBindVertexArray(vertArrObj);
  glBindVertexArray(colorArrObj);

  glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	int h_posV = prog->getAttribute("vertPos");
  GLSL::enableVertexAttribArray(h_posV);

  glVertexAttribPointer(h_posV, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, colorBuffObj);
	int h_posC = prog->getAttribute("pColor");
  GLSL::enableVertexAttribArray(h_posC);
  glVertexAttribPointer(h_posC, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(h_posV, 1);
  glVertexAttribDivisor(h_posC, 1);
  // Draw the points !
  glDrawArraysInstanced(GL_POINTS, 0, 1, numP);

  glVertexAttribDivisor(h_posV, 0);
  glVertexAttribDivisor(h_posC, 0);	

	glDisableVertexAttribArray(h_posV); // 0
  glDisableVertexAttribArray(h_posC); // 1?
}

void particleSys::update() {

  vec3 pos;
  vec4 col;

  //update the particles
  for(auto particle : particles) {
    glm::vec3 new_force = F(particle->getPosition(), *particle);
    particle->update(t, h, new_force);
  }
  t += h;
 
  // Sort the particles by Z
  //temp->rotate(camRot, vec3(0, 1, 0));
  //be sure that camera matrix is updated prior to this update
  vec3 s, t, sk;
  vec4 p;
  quat r;
  glm::decompose(theCamera, s, r, t, sk, p);
  sorter.C = glm::toMat4(r); 
  sort(particles.begin(), particles.end(), sorter);


  //go through all the particles and update the CPU buffer
   for (int i = 0; i < numP; i++) {
        pos = particles[i]->getPosition();
        col = particles[i]->getColor();
        points[i*3+0] =pos.x; 
        points[i*3+1] =pos.y; 
        points[i*3+2] =pos.z; 

        pointColors[i*4+0] =col.r;
        pointColors[i*4+1] =col.g;
        pointColors[i*4+2] =col.b;
        pointColors[i*4+3] =col.a;
			
  } 

  //update the GPU data
   glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
   glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numP*3, points);
   glBindBuffer(GL_ARRAY_BUFFER, colorBuffObj);
   glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numP*4, pointColors);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void particleSys::setOrigin(glm::vec3 source)
{
  if(source != start)
  {
    this->start = source;
    for(auto particle : this->particles)
    {
      particle->rebirth(t);
    }
  }
}

void particleSys::setDist(dist_t Distribution)
{
  dist = Distribution;
}

void particleSys::setColorDist(dist_t color_Dist)
{
  this->colorDist = color_Dist;
}
