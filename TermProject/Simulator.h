#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "particle_system.h"
#include "mass_cloth.h"
#include "rigidbody_system.h"

#define NUMBER_OF_PARTICLES 20

class Simulator
{
public:
	particle_system ParticleSystem;
	float timsStep;
	vec3 ground;
	mass_cloth *cloth;
	rigidbody_system RigidSystem;

	void Initialize();
	void Update();
	void Render();
	void Lighting();
	void DrawGround();
	void Hybrid_Collision();
};