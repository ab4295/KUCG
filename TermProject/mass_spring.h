#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include "Node.h"

using namespace std;
using namespace glm;

class mass_spring
{
public:
	Node *node1;
	Node *node2;
	float length_orig;
	float damp_coef;
	float spring_coef;

	mass_spring(Node *n1, Node *n2);
	void internal_force(double dt);
	void draw();
};

