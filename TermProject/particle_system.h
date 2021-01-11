#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"
#include "Hashtable.h"

using namespace glm;
using namespace std;

class particle_system
{
public:
	vector<particle> particles;
	vec3 gravity_point;
	Hashtable HashT;

	void init(int n);
	void set_gravity(vec3 gravity);
	void Movement(float time);
	void Collision();
	void draw();
	void new_element();
	void refresh();
};

