#include "particle_system.h"


void particle_system::init(int n)
{
	particles.clear();
	for (int i = 0; i < n; i++) {
		particle temp;
		particles.push_back(temp);
	}
	for (int i = 0; i < particles.size(); i++) {
		particles[i].init();
	}
}

void particle_system::new_element()
{
	particle temp;
	temp.init();
	particles.push_back(temp);
}

void particle_system::refresh()
{
	for (int i = particles.size() - 1; i >= 0; i--) {
		if (particles[i].life <= 0)
			particles.erase(particles.begin() + i);
	}
}

void particle_system::set_gravity(vec3 gravity)
{
	gravity_point = gravity;
}

void particle_system::Movement(float time)
{
	for (int i = 0; i < particles.size(); i++) {
		vec3 force = gravity_point;
		particles[i].Movement(time, force);
	}
}

void particle_system::Collision()
{
	HashT.HashTable(particles);

	for (int i = 0; i < particles.size(); i++) {
		particles[i].Collision_Boundary();
		vector<int> res = HashT.GetNeighbor(particles[i].position.x, particles[i].position.y, particles[i].position.z, RADIUS);
		
		for (int k = 0; k < res.size(); k++) {
			int j = res[k];
			
			if (j != i) {
				vec3 pos_ij = particles[j].position - particles[i].position;
				vec3 pos_ji = particles[i].position - particles[j].position;
				double distance = glm::length(pos_ij);
				double collideDist = RADIUS + RADIUS;
				if (distance <= collideDist) {
					// printf("col\n");
					vec3 normal1 = normalize(pos_ij);
					vec3 normal2 = normalize(pos_ji);
					vec3 Vn1 = normal1 * dot(normal1, particles[i].velocity);
					vec3 Vn2 = normal2 * dot(normal2, particles[j].velocity);

					vec3 Vt1 = particles[i].velocity - Vn1;
					vec3 Vt2 = particles[j].velocity - Vn2;

					vec3 relVel = particles[j].velocity - particles[i].velocity;
					if (dot(relVel, pos_ij) < 0) {
						particles[i].velocity = (2 * particles[j].mass * Vn2 + Vn2 * (particles[i].mass - particles[j].mass)) / (particles[i].mass + particles[j].mass) + Vt1;
						particles[j].velocity = (2 * particles[i].mass * Vn1 + Vn1 * (particles[j].mass - particles[i].mass)) / (particles[i].mass + particles[j].mass) + Vt2;
					}
				}
			}
		}
	}
}

void particle_system::draw()
{
	glEnable(GL_POINT_SMOOTH);
	for (int i = 0; i < particles.size(); i++) {
		if (particles[i].life > 0 || true) {
			vec3 pos = particles[i].position;
			vec3 color = vec3(1, 1, 1);
			float size;
			if (particles[i].life > 5) size = 5.0;
			else size = particles[i].life;
			// printf("%f %f %f", pos.x, pos.y, pos.z);
			glColor3f(color.x, color.y, color.z);
			glPointSize(size);
			glBegin(GL_POINTS);
			glVertex3f(pos.x, pos.y, pos.z);
			glEnd();
		}
	}
}