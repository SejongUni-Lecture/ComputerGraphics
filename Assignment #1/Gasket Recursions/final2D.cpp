#include<iostream>
#include "Angel.h"

/*********************************************/
//			2D Gasket programming 
//			  github@ameliacode
/*********************************************/
using namespace Angel;
using namespace std;

typedef vec2 point2;

const int NumTimesToSubdivide = 5;
const int NumTriangles = 729;
const int NumVertices = 3 * NumTriangles;

vec2 points[NumVertices];
int Index = 0;

void triangle(const vec2& a, const vec2& b, const vec2& c) {
	points[Index++] = a;
	points[Index++] = b;
	points[Index++] = c;
}

void divide_triangle(const vec2& a, const vec2& b, const vec2& c, int count) {
	if (count > 0) {
		vec2 v0 = (a + b) / 2.0;
		vec2 v1 = (a + c) / 2.0;
		vec2 v2 = (b + c) / 2.0;
		divide_triangle(a, v0, v1, count - 1);
		divide_triangle(c, v1, v2, count - 1);
		divide_triangle(b, v2, v0, count - 1);
	}
	else triangle(a, b, c);
}

void init(void) {

	//cout << "start gasket" << endl;
	vec2 vertices[3] = { vec2(-1.0,-1.0),vec2(0.0,1.0),vec2(1.0,-1.0) };

	//cout << "loading..." << endl;
	divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide);

	//cout << "vertex buffer..." << endl;
	GLuint abuffer;					//vertex array
	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);

	//cout << "making buffer object..." << endl;
	GLuint buffer;					//buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	//cout << "done!" << endl;
	GLuint Program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(Program);

	//cout << "make shader func" << endl;
	GLuint loc;
	loc = glGetAttribLocation(Program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0);	//white background
}

int main() {
	GLFWwindow* window;
	if (!glfwInit())return -1;
	window = glfwCreateWindow(512, 512, "Gasket", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) cout << "Error" << endl;

	init();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}