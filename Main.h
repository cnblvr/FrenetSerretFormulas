#ifndef __Main_h__
#define __Main_h__

#include <cmath>
#pragma comment(linker, "/subsystem:windows")
#pragma comment(linker, "/entry:\"mainCRTStartup\"")
#pragma comment(lib, "glutBin\\glut32.lib")
#include "glutBin\glut.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const char* WINDOW_TITLE = "движение репера по кривой в пространстве";
const float TRANSLATE_STEP = 0.1;
const float ROTATE_STEP = 5.0;
const float SCALE_STEP = 0.1;
const float LENGTH_UNIT = 100.0;
const float RIGHT_HAND_RULE_X = 90+20, RIGHT_HAND_RULE_Y = 180, RIGHT_HAND_RULE_Z = 180-20;
const float CONST_SCALE = 2.5;
const float PI = 3.14159265;
const float DT = 0.05; // шаг разбиения

namespace r {
	const float a = 1;
	const float b = 1;
	float x(float t);
	float y(float t);
	float z(float t);
	namespace T {
		float x(float t);
		float y(float t);
		float z(float t);
	}
	namespace N {
		float x(float t);
		float y(float t);
		float z(float t);
	}
	namespace B {
		float x(float t);
		float y(float t);
		float z(float t);
	}
};

void onDraw();
void onResizeWindow(int width, int height);
void onKeyboard(unsigned char key, int x, int y);
void onSpecialKeyboard(int key, int x, int y);
void onStartApplication();
void drawHelix(float a, float b, float var);
float f_scale(float scale); // функция, которая преобразует scale в "красивый" scale: (-inf;+inf) -> (0;+inf)
void drawTNB(float t);
void drawText(const char *text, int length, int x, int y);
void onTimer(int value);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);

#endif // __Main_h__