#include "Simulator.h"

using namespace glm;

void Simulator::Initialize()
{
	timsStep = 0.1;
	ground = vec3(0.0, 0.0, 0.0);
	cloth = new mass_cloth();

	cloth->dx = 2;
	cloth->dy = 2;
	cloth->dz = 2;
	cloth->size_x = 20;
	cloth->size_y = 20;
	cloth->size_z = 1;
	cloth->structural_coef = 1500;
	cloth->shear_coef = 50;
	cloth->bending_coef = 1000;
	cloth->iteration_n = 20;
	cloth->drawMode = 0;
	cloth->init();

	ParticleSystem.init(NUMBER_OF_PARTICLES);
	ParticleSystem.set_gravity(vec3(0, -9.8, 0));

	RigidSystem.init();
	RigidSystem.set_gravity(vec3(0, -9.8, 0));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

void Simulator::Update()
{
	vec3 gravity(0.0, -9.8 / cloth->iteration_n, 0.0);

	for (int iter = 0; iter < cloth->iteration_n; iter++) {
		cloth->compute_force(timsStep / cloth->iteration_n, gravity);
		cloth->integrate(timsStep / cloth->iteration_n);
		cloth->collision_response(ground);
	}

	cloth->computeNormal();

	ParticleSystem.Movement(timsStep);
	ParticleSystem.Collision();
	ParticleSystem.refresh();
	ParticleSystem.new_element();

	RigidSystem.Movement(timsStep);
	RigidSystem.Collision();

	Hybrid_Collision();
}

void Simulator::Lighting()
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float light_pos[] = { 150.0, 150.0, 0.0, 1.0 };
	float light_dir[] = { -1, -1, 0, 0.0 };
	float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float frontColor[] = { 0.8, 0.8, 0.8, 1 };

	float matShininess = 20;
	float noMat[] = { 0, 0, 0, 1 };

	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void Simulator::Render()
{
	DrawGround();
	cloth->draw();
	// Drawboundary();

	ParticleSystem.draw();

	RigidSystem.draw();

	glPopMatrix();
}

void Simulator::DrawGround(void)
{
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-50.0f + 50.0f / 64 * x, ground.y, -50.0f + 50.0f / 64 * y);
			glVertex3f(-50.0f + 50.0f / 64 * (x + 1), ground.y, -50.0f + 50.0f / 64 * y);
			glVertex3f(-50.0f + 50.0f / 64 * (x + 1), ground.y, -50.0f + 50.0f / 64 * (y + 1));
			glVertex3f(-50.0f + 50.0f / 64 * x, ground.y, -50.0f + 50.0f / 64 * (y + 1));
		}
	}
	glEnd();
}

void Simulator::Hybrid_Collision()
{
	// 1: rigid v deform
	for (int i = 0; i < RigidSystem.bodies.size(); i++) {
		rigidbody *rigid = RigidSystem.bodies[i];
		vec3 v_rigid = vec3(0, 0, 0);
		int v_rigid_acc = 0;
		// printf("%f %f %f\n", rigid->position.x, rigid->position.y, rigid->position.z);
		
		for (int j = 0; j < cloth->nodes.size(); j++) {
			Node *cloth_node = cloth->nodes[j];
			vec3 pos_ij = cloth_node->position - rigid->position;
			vec3 pos_ji = rigid->position - cloth_node->position;
			double distance = length(pos_ij);
			double collideDist = rigid->radius;
			float eps = 0.75;

			if (distance <= collideDist + eps) {
				// printf("col\n");

				vec3 normal1 = normalize(pos_ij);
				vec3 normal2 = normalize(pos_ji);
				vec3 Vn1 = normal1 * dot(normal1, rigid->velocity);
				vec3 Vn2 = normal2 * dot(normal2, cloth_node->velocity);

				vec3 Vt1 = rigid->velocity - Vn1;
				vec3 Vt2 = cloth_node->velocity - Vn2;

				cloth_node->position = rigid->position + (rigid->radius + eps) * normal1;

				vec3 relVel = cloth_node->velocity - rigid->velocity;
				if (dot(relVel, pos_ij) < 0) {
					// v_rigid += (2 * cloth_node->mass * Vn2 + Vn2 * (rigid->mass - cloth_node->mass)) / (rigid->mass + cloth_node->mass) + Vt1;
					v_rigid += Vt1;
					v_rigid_acc++;
					cloth_node->velocity = (2 * rigid->mass * Vn1 + Vn1 * (cloth_node->mass - rigid->mass)) / (rigid->mass + cloth_node->mass) + Vt2;
					// cloth_node->velocity = Vt2;
					
					// cloth_node->velocity = vec3(0, 0, 0);
				}

			}

		}

		
		if (v_rigid_acc != 0)
			rigid->velocity = v_rigid / (float) v_rigid_acc;
		
	}

	// 2: particle v rigid
	for (int i = 0; i < ParticleSystem.particles.size(); i++) {
		particle *cur_part = &ParticleSystem.particles[i];
		for (int j = 0; j < RigidSystem.bodies.size(); j++) {
			rigidbody *rigid = RigidSystem.bodies[j];
			vec3 pos_ij = rigid->position - cur_part->position;
			vec3 pos_ji = cur_part->position - rigid->position;
			double distance = length(pos_ij);
			double collideDist = rigid->radius;
			// float eps = 0.5;

			if (distance <= collideDist) {
				// printf("col\n");

				vec3 normal1 = normalize(pos_ij);
				vec3 normal2 = normalize(pos_ji);
				vec3 Vn1 = normal1 * dot(normal1, cur_part->velocity);
				vec3 Vn2 = normal2 * dot(normal2, rigid->velocity);

				vec3 Vt1 = cur_part->velocity - Vn1;
				vec3 Vt2 = rigid->velocity - Vn2;

				vec3 relVel = rigid->velocity - cur_part->velocity;

				// cur_part.position = rigid->position + rigid->radius * normal1;
				if (dot(relVel, pos_ij) < 0) {
					cur_part->velocity = (2 * rigid->mass * Vn2 + Vn2 * (cur_part->mass - rigid->mass)) / (rigid->mass + cur_part->mass) + Vt1;
					// printf("%f %f %f\n", cur_part->velocity.x, cur_part->velocity.y, cur_part->velocity.z);
					// printf("%f %f %f\n", ParticleSystem.particles[i].velocity.x, ParticleSystem.particles[i].velocity.y, ParticleSystem.particles[i].velocity.z);
				}
			}
		}
	}

	// 3: particle v cloth
	for (int i = 0; i < ParticleSystem.particles.size(); i++) {
		particle *cur_part = &ParticleSystem.particles[i];
		for (int j = 0; j < cloth->faces.size(); j++) {
			Face *cur_face = cloth->faces[j];
			Node *node1 = cur_face->node1;
			Node *node2 = cur_face->node2;
			Node *node3 = cur_face->node3;
			vec3 center = (node1->position + node2->position + node3->position) / (float) 3;
			vec3 normal = cur_face->normal;
			float thres = 0.5;
			bool is_col = (dot((cur_part->position - center), normal) < thres) && (length(center - cur_part->position) < thres) && (length(cur_part->velocity) > thres);

			if (is_col) {
				vec3 vn = normal * dot(normal, cur_part->velocity);
				// printf("%f %f %f\n", vn.x, vn.y, vn.z);
				vec3 vt = cur_part->velocity - vn;
				cur_part->velocity = vt - 0.1f * vn;
			}
		}
	}
}
