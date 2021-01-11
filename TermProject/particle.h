#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>

#define LENGTH 30
#define RADIUS 0.1

using namespace glm;

class particle
{
public:
	float mass;
	vec3 velocity;
	vec3 position;
	float life;

	void init();
	void Movement(float t, vec3 force);
	void Collision_Boundary();
};

