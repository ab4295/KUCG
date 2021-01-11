#include "rigidbody_system.h"

float rand_float_()
{
	float value = rand() / float(RAND_MAX);
	return value;
}

void rigidbody_system::init()
{
	bodies.clear();
	vector<pair<vec3, float> > init_data;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			init_data.push_back(make_pair(vec3(-40 + 80 / 4 * i, 6, -40 + 80 / 4 * j), 2 + 3 * rand_float_()));
		}
	}

	for (int i = 0; i < init_data.size(); i++) {
		rigidbody * tbody = new rigidbody(init_data[i].first, init_data[i].second);
		tbody->add_force(vec3(-1000 + 2000 * rand_float_(), 1000 * rand_float_(), -1000 + 2000 * rand_float_()));
		// printf("%f\n", tbody->position.y);
		bodies.push_back(tbody);
	}
}

void rigidbody_system::set_gravity(vec3 gravity)
{
	gravity_point = gravity;
}

void rigidbody_system::Movement(float time)
{
	for (int i = 0; i < bodies.size(); i++) {
		vec3 force = gravity_point;
		bodies[i]->Movement(time, force);
	}
}

void rigidbody_system::Collision()
{
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i]->Collision_Boundary();
		for (int j = 0; j < bodies.size(); j++) {
			if (j != i) {
				vec3 pos_ij = bodies[j]->position - bodies[i]->position;
				vec3 pos_ji = bodies[i]->position - bodies[j]->position;
				double distance = length(pos_ij);
				double collideDist = bodies[i]->radius + bodies[j]->radius;
				if (distance <= collideDist) {
					vec3 normal1 = normalize(pos_ij);
					vec3 normal2 = normalize(pos_ji);
					vec3 Vn1 = normal1 * dot(normal1, bodies[i]->velocity);
					vec3 Vn2 = normal2 * dot(normal2, bodies[j]->velocity);

					vec3 Vt1 = bodies[i]->velocity - Vn1;
					vec3 Vt2 = bodies[j]->velocity - Vn2;

					vec3 relVel = bodies[j]->velocity - bodies[i]->velocity;
					if (dot(relVel, pos_ij) < 0) {
						bodies[i]->velocity = (2 * bodies[j]->mass * Vn2 + Vn2 * (bodies[i]->mass - bodies[j]->mass)) / (bodies[i]->mass + bodies[j]->mass) + Vt1;
						bodies[j]->velocity = (2 * bodies[i]->mass * Vn1 + Vn1 * (bodies[j]->mass - bodies[i]->mass)) / (bodies[i]->mass + bodies[j]->mass) + Vt2;
					}
				}
			}
		}
	}
}

void rigidbody_system::draw()
{
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i]->draw();
	}
}