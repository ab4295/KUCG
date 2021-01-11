#include "Viewer.h"

void Viewer::Initialize(void)
{
	S_Simulator.Initialize();
	S_Simulator.Lighting();
	// texture mapping here?
	m_start = true;

	m_Zoom = 125;
	m_Translate[0] = 0.0;
	m_Translate[1] = 0.0;
	m_Rotate[0] = 30.0;
	m_Rotate[1] = 0.0;
}

void Viewer::Update()
{
	if (m_start == true) {
		S_Simulator.Update();
	}

	m_Rotate[1] += 1;
	if (m_Rotate[1] > 360) m_Rotate[1] = 0;
}

void Viewer::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -m_Zoom);
	glTranslatef(m_Translate[0], m_Translate[1], 0.0);
	glPushMatrix();
	glRotatef(m_Rotate[0], 1.0, 0.0, 0.0);
	glRotatef(m_Rotate[1], 0.0, 1.0, 0.0);
	S_Simulator.Render();
	glPopMatrix();
	float light_pos[] = { 150.0, 150.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glutSwapBuffers();
	glutPostRedisplay();
}

void Viewer::Reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);
	glViewport(0, 0, w, h);
}