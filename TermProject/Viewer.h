#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Simulator.h"

class Viewer
{
public:
	Simulator S_Simulator;
	bool m_start;
	float m_Zoom;
	float m_Translate[2];
	float m_Rotate[2];

	void Initialize();
	void Update();
	void Render();
	void Reshape(int w, int h);
};