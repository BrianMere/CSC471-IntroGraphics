//
// sueda
// November, 2014/ wood 16
//

#pragma once

#ifndef LAB471_PARTICLE_H_INCLUDED
#define LAB471_PARTICLE_H_INCLUDED

#include <vector>
#include <memory>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

float randFloat(float l, float h);

class Particle
{
public:

	// public characteristics of our particle
	float charge; // +1 or -1
	float m; // mass
	float d; // viscous damping

	Particle(vec3 pos);
	virtual ~Particle();
	void load();
	void rebirth(float t);
	void update(float t, float h, const glm::vec3 &g);
	const vec3 &getPosition() const { return x; };
	const vec3 &getVelocity() const { return v; };
	const vec4 &getColor() const { return color; };

	inline void setStartingVelocity(glm::vec3 velocity)
	{
		this->v = velocity;
	}

	inline void setColor(glm::vec3 color)
	{
		this->color = glm::vec4(color.x, color.y, color.z, 1.0f);
	}

	inline glm::vec4 getColor()
	{
		return this->color;
	}
	
private:

	vec3 x; // position
	vec3 v; // velocity
	float lifespan; // how long this particle lives
	float tEnd;     // time this particle dies
	vec4 color;
	vec3 start; // starting position.
};

#endif // LAB471_PARTICLE_H_INCLUDED
