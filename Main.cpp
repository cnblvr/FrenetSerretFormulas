#include "Main.h"

int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;

float rotateX = RIGHT_HAND_RULE_X, rotateY = RIGHT_HAND_RULE_Y, rotateZ = RIGHT_HAND_RULE_Z;
float translateX = 0, translateY = 0;
float scale = CONST_SCALE; // масштаб
bool help = false, info = false;
float varT = 0;
bool pause = true;
bool move = false;
int mouseMoveX = -1, mouseMoveY = -1;

void onDraw()
{
	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glTranslatef(translateX, translateY, 0);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateZ, 0, 0, 1);
	glScalef(f_scale(scale), f_scale(scale), f_scale(scale));

	glBegin(GL_LINES);
		// ось x
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(LENGTH_UNIT, 0, 0);
		// ось y
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, LENGTH_UNIT, 0);
		// ось z
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, LENGTH_UNIT);
	glEnd();

	glBegin(GL_LINES);
		// ось x
		glColor3f(0.5, 0, 0);
		glVertex3f(-LENGTH_UNIT, 0, 0);
		glVertex3f(0, 0, 0);
		// ось y
		glColor3f(0, 0.5, 0);
		glVertex3f(0, -LENGTH_UNIT, 0);
		glVertex3f(0, 0, 0);
		// ось z
		glColor3f(0, 0, 0.5);
		glVertex3f(0, 0, -LENGTH_UNIT);
		glVertex3f(0, 0, 0);
	glEnd();
	///*

	//*/
	drawHelix(0, 4*PI, varT);
	glPopMatrix();
	if(help) {
		drawText("[h] - show/hide help", 20, 4, windowHeight - 12-1);
		drawText("[i] - show/hide info", 20, 4, windowHeight - (12+2)*2);
		drawText("left and right - rotation of the plane Oxy", 42, 4, windowHeight - (12+2)*3);
		drawText("up and down - rotation of the plane Oyz", 39, 4, windowHeight - (12+2)*4);
		drawText("[,] and [.] - rotation of the plane Oxz", 39, 4, windowHeight - (12+2)*5);
		drawText("[-] and [=] - zoom in or out", 28, 4, windowHeight - (12+2)*6);
		drawText("[n] - properly set the camera", 29, 4, windowHeight - (12+2)*7);
		drawText("escape or [q] - exit", 22, 4, windowHeight - (12+2)*8);
	} else
		drawText("[h]", 3, 4, windowHeight - 12 - 1);
	if(info) {
		glColor3f(1, 1, 1);
		drawText("helix is a curve in three-dimensional space", 43, 4, 6);
		glColor3f(1, 0, 0);
		drawText("Ox", 2, 4, 6 + (12+2));
		glColor3f(0, 1, 0);
		drawText("Oy", 2, 4 + 20, 6 + (12+2));
		glColor3f(0, 0, 1);
		drawText("Oz", 2, 4 + 40, 6 + (12+2));
		glColor3f(1, 1, 0);
		drawText("B - the binormal unit vector", 27, 4, 6 + (12+2)*2);
		glColor3f(1, 0, 1);
		drawText("N - the normal unit vector", 26, 4, 6 + (12+2)*3);
		glColor3f(0, 1, 1);
		drawText("T - the unit vector tangent", 27, 4, 6 + (12+2)*4);
	}
	glFlush();
	glutSwapBuffers();
}

void onResizeWindow(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(0, width/height, -500, 500);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	onDraw();
}

void onKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case ',':
		rotateY -= ROTATE_STEP;
		break;
	case '.':
		rotateY += ROTATE_STEP;
		break;
	case '-':
		scale += SCALE_STEP;
		if(scale == 0)
			scale = 1;
		break;
	case '=':
		scale -= SCALE_STEP;
		if (scale == 0)
			scale = -1;
		break;
	case 'n':
		rotateX = RIGHT_HAND_RULE_X;
		rotateY = RIGHT_HAND_RULE_Y;
		rotateZ = RIGHT_HAND_RULE_Z;
		scale = 1;
		break;
	case 'h':
		help = !help;
		break;
	case 'i':
		info = !info;
		break;
	case ' ':
		pause = !pause;
		onTimer(0);
		break;
	case 27:
	case 'q':
	case 'Q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void onSpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		rotateZ += ROTATE_STEP;
		break;
	case GLUT_KEY_RIGHT:
		rotateZ -= ROTATE_STEP;
		break;
	case GLUT_KEY_UP:
		translateY -= TRANSLATE_STEP;
		break;
	case GLUT_KEY_DOWN:
		translateY += TRANSLATE_STEP;
		break;
	}
	glutPostRedisplay();
}

void onStartApplication()
{
	onResizeWindow(windowWidth, windowHeight);
}

void drawHelix(float a, float b, float var)
{
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	for(float t=a; t<=b; t+=DT)
	{
		glVertex3f(r::x(t), r::y(t), r::z(t));
	}
	glEnd();
	drawTNB(var);
}

float f_scale(float scale)
{
	if(scale < 0)
		return -scale + 1;
	else
		return std::exp(-scale);
}

void drawTNB(float t)
{
	glBegin(GL_LINES);
		// единичный касательный вектор
		glColor3f(0, 1, 1);
		glVertex3f(r::x(t), r::y(t), r::z(t));
		glVertex3f(r::T::x(t)+r::x(t), r::T::y(t)+r::y(t), r::T::z(t)+r::z(t));
		// единичный вектор главной нормали
		glColor3f(1, 0, 1);
		glVertex3f(r::x(t), r::y(t), r::z(t));
		glVertex3f(r::N::x(t)+r::x(t), r::N::y(t)+r::y(t), r::N::z(t)+r::z(t));
		// единичный вектор бинормали
		glColor3f(1, 1, 0);
		glVertex3f(r::x(t), r::y(t), r::z(t));
		glVertex3f(r::B::x(t)+r::x(t), r::B::y(t)+r::y(t), r::B::z(t)+r::z(t));
	glEnd();
}

void onTimer(int value)
{
	if(!pause) {
		varT += 0.01;
		glutPostRedisplay();
		glutTimerFunc(40, onTimer, 0);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition((SCREEN_WIDTH-windowWidth)/2, (SCREEN_HEIGHT-windowHeight)/2);
	glutCreateWindow(WINDOW_TITLE);
	glEnable(GL_DEPTH_TEST);
	onStartApplication();
	glutDisplayFunc(onDraw);
	glutReshapeFunc(onResizeWindow);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialKeyboard);
	glutTimerFunc(40, onTimer, 0);
	glutMainLoop();
	return 0;
}

float r::x(float t)
{
	return a*std::cos(t);
}
float r::y(float t)
{
	return a*std::sin(t);
}
float r::z(float t)
{
	return b*t;
}
float r::T::x(float t)
{
	return (-a*std::sin(t)) / sqrt(a*a + b*b);
}
float r::T::y(float t)
{
	return (a*std::cos(t)) / sqrt(a*a + b*b);
}
float r::T::z(float t)
{
	return b / sqrt(a*a + b*b);
}
float r::N::x(float t)
{
	return -std::cos(t);
}
float r::N::y(float t)
{
	return -std::sin(t);
}
float r::N::z(float t)
{
	return 0;
}
float r::B::x(float t)
{
	return (a*b*std::sin(t)) / sqrt(a*a*b*b + a*a*a*a);
}
float r::B::y(float t)
{
	return (-a*b*std::cos(t)) / sqrt(a*a*b*b + a*a*a*a);
}
float r::B::z(float t)
{
	return a*a / sqrt(a*a*b*b + a*a*a*a);
}

void drawText(const char *text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, windowWidth, 0, windowHeight, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2f(x, y);
	for (int i = 0; i<length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}



/*

void DrawOctagon()
{
	static const float STEP = PI / 4;
	glColor4f(octagon->red, octagon->green, octagon->blue, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(octagon->xTranslate, octagon->yTranslate, 0);
	glRotatef(octagon->aRotate, 0.0, 0.0, 1.0);
	glScalef(octagon->xScale, octagon->yScale, 1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	for (int i = 0; i<8; i++) {
		glVertex2f(octagon->point[i]->x, octagon->point[i]->y);
	}
	glEnd();
}




*/