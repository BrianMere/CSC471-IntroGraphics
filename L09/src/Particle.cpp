//
// sueda - geometry edits Z. Wood
// 3/16
//

#include <iostream>
#include "Particle.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"


float randFloat(float l, float h)
{
	float r = rand() / (float) RAND_MAX;
	return (1.0f - r) * l + r * h;
}

Particle::Particle(vec3 start) :
	charge(1.0f),
	m(1.0f),
	d(0.0f),
	x(start),
	v(0.0f, 0.0f, 0.0f),
	lifespan(1.0f),
	tEnd(0.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	start(start)
{
}

Particle::~Particle()
{
}

void Particle::load()
{
	// Random initialization
	rebirth(0.0f);
}

/* all particles born at the origin */
void Particle::rebirth(float t)
{
	charge = randFloat(0.0f, 1.0f) < 0.5 ? -1.0f : 1.0f;	
	m = 1.0f;
  	d = randFloat(0.0f, 0.02f);
	x = start;

	lifespan = randFloat(100.0f, 200.0f); 
	tEnd = t + lifespan;
}

void Particle::update(float t, float h, const vec3 &g)
{
	if(t > tEnd) {
		rebirth(t);
	}

	//very simple update
	x += h*v; // update the position based on velocity
	v += h*g / m; // update the velocity based on forces, over their mass (for their actual acceleration)

	color.a = (tEnd-t)/lifespan; // let this particle "die out"
}
