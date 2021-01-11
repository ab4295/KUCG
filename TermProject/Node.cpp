#include "Node.h"

Node::Node(vec3 v)
{
	mass = 1.0;
	isFixed = false;
	position = v;
	velocity = vec3(0.0, 0.0, 0.0);
	force = vec3(0, 0, 0);
	tex_coord = vec2(0, 0);
}

Node::Node(vec3 v, vec2 tex)
{
	mass = 1.0;
	isFixed = false;
	position = v;
	velocity = vec3(0.0, 0.0, 0.0);
	force = vec3(0, 0, 0);
	tex_coord = tex;
}

void Node::add_force(vec3 additional_force)
{
	force += additional_force;
}

void Node::integrate(float dt)
{
	if (!isFixed) {
		vec3 vc = force / mass;
		// printf("%f %f %f\n", vc.x, vc.y, vc.z);
		velocity += vc * dt;
		// printf("%f %f %f\n", velocity.x, velocity.y, velocity.z);
		position += velocity * dt;
		// printf("%f %f %f\n", position.x, position.y, position.z);
	}

	force = vec3(0, 0, 0);
}

void Node::draw()
{
	glEnable(GL_POINT_SMOOTH);
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glVertex3f(position.x, position.y, position.z);
	glEnd();
}

Face::Face(Node *n1, Node *n2, Node *n3)
{
	node1 = n1;
	node2 = n2;
	node3 = n3;
	normal = vec3(0, 0, 0);
}