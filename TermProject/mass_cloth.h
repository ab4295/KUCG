#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "Node.h"
#include "mass_spring.h"

#define DRAW_MASS_NODES 1
#define DRAW_SPRINGS 2
#define DRAW_FACES 3

using namespace std;
using namespace glm;

class mass_cloth
{
public:
	vector<Node *> nodes;
	vector<mass_spring *> spring;
	vector<Face *> faces;

	int size_x, size_y, size_z;
	double dx, dy, dz;
	double structural_coef;
	double shear_coef;
	double bending_coef;
	int iteration_n;
	int drawMode;
	GLuint mTexture;

	void init();
	void compute_force(float dt, vec3 gravity);
	void integrate(float dt);
	void collision_response(vec3 ground);
	void computeNormal();
	void draw();
	bool LoadMeshFromFile(char *filename, char *texFile);
};

