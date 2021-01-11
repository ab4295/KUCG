#include "mass_spring.h"

mass_spring::mass_spring(Node *n1, Node *n2)
{
	node1 = n1;
	node2 = n2;
	length_orig = length(n1->position - n2->position);
	damp_coef = 10;
	// spring_coef = 0.8;
}

void mass_spring::internal_force(double dt)
{
	vec3 x1 = node2->position - node1->position;
	vec3 dir1 = normalize(x1);
	vec3 v1 = node2->velocity - node1->velocity;
	vec3 force1 = dir1 * (spring_coef * (length(x1) - length_orig)
		+ damp_coef * (v1 * dir1));
	node1->add_force(force1);
	
	vec3 x2 = node1->position - node2->position;
	vec3 dir2 = normalize(x2);
	vec3 v2 = node1->velocity - node2->velocity;
	vec3 force2 = dir2 * (spring_coef * (length(x2) - length_orig)
		+ damp_coef * (v2 * dir2));
	node2->add_force(force2);
	/*
	vec3 x1 = node1->position - node2->position;
	vec3 dir1 = normalize(x1);
	vec3 v1 = node1->velocity - node2->velocity;
	vec3 force1 = dir1 * (spring_coef * (glm::length(x1) - length)
		+ damp_coef * dot(v1, dir1));
	node1->add_force(force1);

	vec3 x2 = node2->position - node1->position;
	vec3 dir2 = normalize(x2);
	vec3 v2 = node2->velocity - node1->velocity;
	vec3 force2 = dir2 * (spring_coef * (glm::length(x2) - length)
		+ damp_coef * dot(v2, dir2));
	node2->add_force(force2);
	*/
}

void mass_spring::draw()
{
	// glEnable(GL_POINT_SMOOTH);
	glColor3f(1.0, 1.0, 1.0);
	// glPointSize(1.0);
	glBegin(GL_LINES);
	glVertex3f(node1->position.x, node1->position.y, node1->position.z);
	glVertex3f(node2->position.x, node2->position.y, node2->position.z);
	glEnd();
}