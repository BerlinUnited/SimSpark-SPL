#include "axis.h"
#include "../openglserver/openglserver.h"

using namespace kerosin;

Axis::Axis()
{
}

void Axis::RenderInternal()
{
	// X
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
	glEnd();

	// Y
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
	glEnd();

	// Z
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0,0,0);
		glVertex3f(0,0,10);
	glEnd();
}