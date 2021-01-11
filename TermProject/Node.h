#pragma once
#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

class Node
{
public:
	vec3 position;
	vec3 force;
	vec3 velocity;
	vec3 normal;
	vec2 tex_coord;
	vector<int> faces;
	float mass;
	bool isFixed;

	Node(vec3 v);
	Node(vec3 v, vec2 tex);
	void add_force(vec3 additional_force);
	void integrate(float dt);
	void draw();
};

class Face
{
public:
	Node *node1, *node2, *node3;
	vec3 normal;

	Face(Node *n1, Node *n2, Node *n3);
};
