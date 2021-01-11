#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "rigidbody.h"

using namespace glm;
using namespace std;

class rigidbody_system
{
public:
	vector<rigidbody *> bodies;
	vec3 gravity_point;

	void init();
	void set_gravity(vec3 gravity);
	void Movement(float time);
	void Collision();
	void draw();
};



