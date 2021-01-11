#include "rigidbody.h"

vec3 param_f(float u, float v, float r)
{
	return vec3(cos(u) * sin(v) * r, cos(v) * r, sin(u) * sin(v) * r);
}

rigidbody::rigidbody(vec3 pos, float r)
{
	position = pos;
	radius = r;
	mass = r * r * r;
	velocity = vec3(0, 0, 0);
	force_in = vec3(0, 0, 0);
	
	// sphere by parametric equation
	// code from https://stackoverflow.com/questions/7687148/drawing-sphere-in-opengl-without-using-glusphere
	float startu = 0;
	float startv = 0;
	float endu = 3.14 * 2;
	float endv = 3.14;
	float uresol = 20;
	float vresol = 20;
	float stepu = (endu - startu) / uresol;
	float stepv = (endv - startv) / vresol;
	for (int i = 0; i < uresol; i++) {
		for (int j = 0; j < vresol; j++) {
			float u = i * stepu + startu;
			float v = j * stepv + startv;
			float un = (i + 1 == uresol) ? endu : (i + 1) * stepu + startu;
			float vn = (j + 1 == vresol) ? endv : (j + 1) * stepv + startv;
			vec3 p0 = param_f(u, v, r);
			vec3 p1 = param_f(u, vn, r);
			vec3 p2 = param_f(un, v, r);
			vec3 p3 = param_f(un, vn, r);
			triangles.push_back({ p0, p2, p1 });
			triangles.push_back({ p3, p1, p2 });
		}
	}
}

void rigidbody::add_force(vec3 additional_force)
{
	force_in += additional_force;
}

void rigidbody::Movement(float t, vec3 force)
{
	vec3 acc = (force + force_in) / mass;
	velocity += acc * t;
	position += velocity * t;

	force_in = vec3(0, 0, 0);
}

void rigidbody::Collision_Boundary()
{
	float COR = 0.5f;
	vec3 lbound = vec3(-50, 0, -50);
	vec3 ubound = vec3(50, 100, 50);
	if (position.x <= lbound.x + radius && velocity.x < 0) {
		position.x = lbound.x + radius;
		velocity.x *= (-1.0f) * COR;
	}
	if (position.x >= ubound.x - radius && velocity.x > 0) {
		position.x = ubound.x - radius;
		velocity.x *= (-1.0f) * COR;
	}
	if (position.y <= lbound.y + radius && velocity.y < 0) {
		position.y = lbound.y + radius;
		velocity.y *= (-1.0f) * COR;
		velocity.x *= 0.9f;
		velocity.z *= 0.9f;
	}
	if (position.y >= ubound.y - radius && velocity.y > 0) {
		position.y = ubound.y - radius;
		velocity.y *= (-1.0f) * COR;
	}
	if (position.z <= lbound.z + radius && velocity.z < 0) {
		position.z = lbound.z + radius;
		velocity.z *= (-1.0f) * COR;
	}
	if (position.z >= ubound.z - radius && velocity.z > 0) {
		position.z = ubound.z - radius;
		velocity.z *= (-1.0f) * COR;
	}
}

void rigidbody::draw()
{
	for (int i = 0; i < triangles.size(); i++) {
		vec3 p1 = triangles[i].p1 + position;
		vec3 p2 = triangles[i].p2 + position;
		vec3 p3 = triangles[i].p3 + position;
		vec3 v1 = p2 - p1;
		vec3 v2 = p3 - p1;
		vec3 n = normalize(cross(v1, v2));

		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);

		glNormal3f(n.x, n.y, n.z);

		glVertex3f(p1.x, p1.y, p1.z);

		glVertex3f(p2.x, p2.y, p2.z);

		glVertex3f(p3.x, p3.y, p3.z);

		glEnd();
	}
}
