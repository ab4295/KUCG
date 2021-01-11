#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Viewer.h"


using namespace std;

Viewer OpenGL_Viewer;

void Initialize(void)
{
	OpenGL_Viewer.Initialize();
}

void Update(int value)
{
	OpenGL_Viewer.Update();
	glutPostRedisplay();
	glutTimerFunc(10, Update, 0);
}

void Render(void)
{
	OpenGL_Viewer.Render();
}

void Reshape(int w, int h)
{
	OpenGL_Viewer.Reshape(w, h);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(1024, 800);
	glutCreateWindow("Term Project Simulation");

	Initialize();
	
	glutTimerFunc(10, Update, 0);
	glutDisplayFunc(Render);

	glutReshapeFunc(Reshape);

	glutMainLoop();
	

	return 0;
}
