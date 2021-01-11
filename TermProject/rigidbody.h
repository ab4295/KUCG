#pragma once
#include <vector>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>


using namespace std;
using namespace glm;

struct tri { vec3 p1, p2, p3; };

class rigidbody
{
public:
	vector<vec3> vertices;
	float radius;
	vec3 position;
	vec3 rotaxis;
	vec3 velocity;
	vec3 force_in;
	float mass;
	vector<tri> triangles;

	rigidbody(vec3 pos, float r);
	void draw();
	void Collision_Boundary();
	void Movement(float t, vec3 force);
	void add_force(vec3 additional_force);
};

