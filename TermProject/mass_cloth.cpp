#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "mass_cloth.h"


void mass_cloth::init()
{
	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++) {
			for (int k = 0; k < size_z; k++) {
				Node *tnode = new Node(vec3(-20 + i * dx, 15, -20 + j * dz), vec2(1.0f * i / size_x, 1.0f * j / size_y));
				nodes.push_back(tnode);
				// tnode->isFixed = true;
				// printf("%f %f %f\n", tnode->position.x, tnode->position.y, tnode->position.z);
			}
		}
	}

	// nodes[0]->isFixed = true;
	// nodes[(size_x - 1) * size_y]->isFixed = true;
	// nodes[size_y - 1]->isFixed = true;
	// nodes[size_x * size_y - 1]->isFixed = true;

	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y - 1; j++) {
			mass_spring *tsp = new mass_spring(nodes[i * size_x + j], nodes[i * size_x + j + 1]);
			tsp->spring_coef = structural_coef;
			spring.push_back(tsp);
		}
	}

	for (int i = 0; i < size_x - 1; i++) {
		for (int j = 0; j < size_y; j++) {
			mass_spring *tsp = new mass_spring(nodes[i * size_x + j], nodes[(i + 1) * size_x + j]);
			tsp->spring_coef = structural_coef;
			spring.push_back(tsp);
		}
	}
	
	for (int i = 0; i < size_x - 1; i++) {
		for (int j = 0; j < size_y - 1; j++) {
			mass_spring *tsp = new mass_spring(nodes[i * size_x + j], nodes[(i + 1) * size_x + j + 1]);
			tsp->spring_coef = shear_coef;
			spring.push_back(tsp);
		}
	}

	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y - 2; j++) {
			mass_spring *tsp = new mass_spring(nodes[i * size_x + j], nodes[i * size_x + j + 2]);
			tsp->spring_coef = bending_coef;
			spring.push_back(tsp);
		}
	}

	for (int i = 0; i < size_x - 2; i++) {
		for (int j = 0; j < size_y; j++) {
			mass_spring *tsp = new mass_spring(nodes[i * size_x + j], nodes[(i + 2) * size_x + j]);
			tsp->spring_coef = bending_coef;
			spring.push_back(tsp);
		}
	}

	for (int i = 0; i < size_x - 1; i++) {
		for (int j = 0; j < size_y - 1; j++) {
			for (int k = 0; k < size_z; k++) {
				Node *n1 = nodes[i * size_x + j];
				Node *n2 = nodes[(i + 1) * size_x + j];
				Node *n3 = nodes[(i + 1) * size_x + j + 1];
				Face *tf1 = new Face(n1, n2, n3);
				faces.push_back(tf1);
				int ind1 = faces.size() - 1;
				n1->faces.push_back(ind1);
				n2->faces.push_back(ind1);
				n3->faces.push_back(ind1);

				Node *n4 = nodes[i * size_x + j];
				Node *n5 = nodes[(i + 1) * size_x + j + 1];
				Node *n6 = nodes[i * size_x + j + 1];
				Face *tf2 = new Face(n4, n5, n6);
				faces.push_back(tf2);
				int ind2 = faces.size() - 1;
				n4->faces.push_back(ind2);
				n5->faces.push_back(ind2);
				n6->faces.push_back(ind2);

				/*
				faces.push_back(nodes[i * size_x + j]);
				faces.push_back(nodes[(i + 1) * size_x + j]);
				faces.push_back(nodes[i * size_x + j + 1]);
				faces_normal.push_back(vec3(0, 0, 0));
				faces_normal.push_back(vec3(0, 0, 0));
				faces_normal.push_back(vec3(0, 0, 0));

				faces.push_back(nodes[i * size_x + j + 1]);
				faces.push_back(nodes[(i + 1) * size_x + j]);
				faces.push_back(nodes[(i + 1) * size_x + j + 1]);
				faces_normal.push_back(vec3(0, 0, 0));
				faces_normal.push_back(vec3(0, 0, 0));
				faces_normal.push_back(vec3(0, 0, 0));
				*/
			}
		}
	}

	// drawMode = DRAW_MASS_NODES;
	// drawMode = DRAW_SPRINGS;
	drawMode = DRAW_FACES;

	glEnable(GL_TEXTURE_2D);

	char tf[] = "christmas.jpg";
	LoadMeshFromFile(tf, tf);
}

void mass_cloth::compute_force(float dt, vec3 gravity)
{
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->add_force(gravity * nodes[i]->mass);
	}

	for (int i = 0; i < spring.size(); i++) {
		spring[i]->internal_force(dt);
	}
}

void mass_cloth::integrate(float dt)
{
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->integrate(dt);
	}
}

void mass_cloth::collision_response(vec3 ground)
{
	bool col_gnd = false;
	vec3 gnd_normal(0, 1, 0);
	ground.y += 0.05;
	float r = 0.01;

	for (int i = 0; i < nodes.size(); i++) {
		Node *cnode = nodes[i];
		col_gnd = (dot((cnode->position - ground), gnd_normal) < r) && (dot(gnd_normal, cnode->velocity) < 0);
		if (col_gnd) {
			cnode->position.y = ground.y;
			cnode->velocity.y = 0;
		}
	}
}

void mass_cloth::computeNormal()
{
	for (int i = 0; i < faces.size(); i++) {
		Node *n1 = faces[i]->node1;
		Node *n2 = faces[i]->node2;
		Node *n3 = faces[i]->node3;

		vec3 p1 = n2->position - n1->position;
		vec3 p2 = n3->position - n1->position;

		vec3 cr = cross(p1, p2);
		faces[i]->normal = normalize(cr);
		// printf("%f %f %f\n", cr.x, cr.y, cr.z);
	}


	for (int i = 0; i < nodes.size(); i++) {
		Node *cn = nodes[i];
		vec3 acc = vec3(0, 0, 0);
		for (int j = 0; j < cn->faces.size(); j++) {
			acc += faces[cn->faces[j]]->normal;
		}
		
		if (length(acc) < 10e-6) cn->normal = vec3(0, 0, 0);
		else cn->normal = normalize(acc / (float) cn->faces.size());
		// printf("%d\n", cn->faces.size());
		// printf("%f %f %f\n", cn->normal.x, cn->normal.y, cn->normal.z);
	}
}

void mass_cloth::draw()
{
	switch (drawMode)
	{
	case DRAW_MASS_NODES:
		glDisable(GL_LIGHTING);
		for (int i = 0; i < nodes.size(); i++)
			nodes[i]->draw();
		glEnable(GL_LIGHTING);
		break;

	case DRAW_SPRINGS:
		glDisable(GL_LIGHTING);
		for (int i = 0; i < spring.size(); i++)
			spring[i]->draw();
		glEnable(GL_LIGHTING);
		break;

	case DRAW_FACES:
		for (int i = 0; i < faces.size(); i++) {
			Node *n1 = faces[i]->node1;
			Node *n2 = faces[i]->node2;
			Node *n3 = faces[i]->node3;
			glColor3f(1, 1, 1);
			glBegin(GL_TRIANGLES);
			
			glNormal3f(n1->normal.x, n1->normal.y, n1->normal.z);

			glTexCoord2f(n1->tex_coord.x, n1->tex_coord.y);
			glVertex3f(n1->position.x, n1->position.y, n1->position.z);
			
			glTexCoord2f(n2->tex_coord.x, n2->tex_coord.y);
			glVertex3f(n2->position.x, n2->position.y, n2->position.z);
			
			glTexCoord2f(n3->tex_coord.x, n3->tex_coord.y);
			glVertex3f(n3->position.x, n3->position.y, n3->position.z);
			
			glEnd();
		}
		break;
		
	default:
		break;
	}
}


bool mass_cloth::LoadMeshFromFile(char *filename, char *texFile)
{
	glGenTextures(1, &mTexture);

	FILE *fp = NULL;
	fopen_s(&fp, texFile, "rb");

	// FILE *fp = fopen_s(texFile, "rb");
	if (!fp) {
		printf("ERROR: No %s.\nfail to bind %d\n", texFile, mTexture);
		return false;
	}

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	return true;
}