#include "particle.h"

float rand_float()
{
	float value = rand() / float(RAND_MAX);
	return value;
}

void particle::init()
{
	mass = 1;
	velocity =
		vec3(rand_float() - 0.5,
			0,
			rand_float() - 0.5);
	position =
		vec3((49 - 98 * rand_float()),
		50,
			(49 - 98 * rand_float()));
	life = 10;
}

void particle::Movement(float t, vec3 force)
{
	vec3 acc = force / mass;
	vec3 turbulence = normalize(vec3(rand_float() - 0.5, rand_float() - 0.5, rand_float() - 0.5));
	velocity += (acc + turbulence) * t;
	velocity *= 0.9;
	position += velocity * t;

	if(velocity.y < 0.01)
		life -= t;
}

void particle::Collision_Boundary()
{
	float COR = 0.1f;
	vec3 lbound = vec3(-50, 0, -50);
	vec3 ubound = vec3(50, 100, 50);

	if (position.x <= lbound.x + RADIUS && velocity.x < 0) {
		position.x = lbound.x + RADIUS;
		velocity.x *= (-1.0f) * COR;
	}
	if (position.x >= ubound.x - RADIUS && velocity.x > 0) {
		position.x = ubound.x - RADIUS;
		velocity.x *= (-1.0f) * COR;
	}
	if (position.y <= lbound.y + RADIUS && velocity.y < 0) {
		position.y = lbound.y + RADIUS;
		velocity.y *= (-1.0f) * COR;
	}
	if (position.y >= ubound.y - RADIUS && velocity.y > 0) {
		position.y = ubound.y - RADIUS;
		velocity.y *= (-1.0f) * COR;
	}
	if (position.z <= lbound.z + RADIUS && velocity.z < 0) {
		position.z = lbound.z + RADIUS;
		velocity.z *= (-1.0f) * COR;
	}
	if (position.z >= ubound.z - RADIUS && velocity.z > 0) {
		position.z = ubound.z - RADIUS;
		velocity.z *= (-1.0f) * COR;
	}
}