#include<iostream>
//#include<stdlib.h>
#include"Angel.h"
#include<InitShader.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#define MAX 1000000

/*********************************************/
//	 3D Gasket programming using triangle
//			github@ameliacode	
/*********************************************/

using namespace std;

typedef vec2 point2;
typedef vec3 point3;
typedef vec3 color3;

int Ndivisions = 7;
int total;
unsigned int Nvertices = 1000000;

int i = 0;
int colorindex;
int axisIndex = 0;
//vec3 points[MAX];
//vec3 colors[MAX];
vec3 *points;
vec3 *colors;
vec3 base_color[5] = { vec3(1.0,0.0,0.0), vec3(1.0,1.0,1.0),
						 vec3(0.0,0.0,1.0), vec3(0.0,1.0,0.0), vec3(0.5,0.5,0.5) };
vec3 base[4] = { {0.0,0.0,1.0},{0.0,0.942809,-0.33333},
					{-0.816497,-0.471405,-0.333333}, {0.816497,-0.471405,-0.333333} };

//좌표축별 회전각
enum { stop = 0, Zaxis = 1, Xaxis = 2, Yaxis = 3 };
int Axis = 0;
int currentpt = 0;

float Theta = 0.0;
float scale = 1.0f;

GLuint theta; //쉐이더 유니폼 변수의 위치 저장
//GLuint scale;

GLuint abuffer;					//vertex buffer
GLuint buffer;					//buffer object
GLuint program;

unsigned int width = 720;
unsigned int height = 720;

//function lists
void init(void);
void triangle(const vec3& a, const vec3& b, const vec3& c);
void tetra(const vec3& a, const vec3& b, const vec3& c, const vec3& d, int count);
void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4);

void idle(void) {
	Theta += 0.1f;
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	glBindVertexArray(abuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUseProgram(program);

	GLuint uTmp = glGetUniformLocation(program, "uTmp");
	glUniform1i(uTmp, 1);

	mat4 rotation;

	if (Axis == 1) {
		rotation = RotateZ(Theta);
	}
	else if (Axis == 2) {
		rotation = RotateX(Theta);
	}
	else if (Axis == 3) {
		rotation = RotateY(Theta);
	}

	mat4 scaling = Scale(scale);
	GLuint uMat = glGetUniformLocation(program, "uMat");

	glUniformMatrix4fv(uMat, 1, GL_FALSE, scaling);
	//glUniform3fv(theta, 1, Theta);
	//glUniform1fv(scale, 1, Scaling);

	glDrawArrays(GL_TRIANGLES, 0, Nvertices);
}

int main() {

	GLFWwindow* window;
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(width, height, "Assignment 2", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) std::cout << "Error" << endl;

	init();
	while (!glfwWindowShouldClose(window)) {
		display();
		idle();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}


void init(void) {

	Nvertices =10* pow(3, Ndivisions + 2); 
	total = sizeof(float) * sizeof(vec3)*Nvertices;

	points = (vec3*)malloc(sizeof(vec3)*total);
	colors = (vec3*)malloc(sizeof(vec3)*total);

	//tetra(vertices[0], vertices[1], vertices[2], vertices[3], Ndivisions);
	setGasketVertices(0, 1, 2, 3);

	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(points) + sizeof(colors))*total, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points)*total, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*total, sizeof(colors)*total, colors);

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	//vPosition
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//vColor
	GLuint loc2 = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)*total));

	theta = glGetUniformLocation(program, "theta");
	//scale = glGetUniformLocation(program, "scale");


	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);	//white background
}

void triangle(const vec3& a, const vec3& b, const vec3& c, int colorindex) {
	colors[i] = base_color[colorindex];
	points[i] = a;
	++i;
	colors[i] = base_color[colorindex];
	points[i] = b;
	++i;
	colors[i] = base_color[colorindex];
	points[i] = c;
	++i;
}

void divide_triangle(const vec3& a, const vec3& b, const vec3& c, int count, int color) {
	if (count > 0) {
		vec3 v0 = (a + b) / 2.0;
		vec3 v1 = (a + c) / 2.0;
		vec3 v2 = (b + c) / 2.0;
		divide_triangle(a, v0, v1, count - 1, color);
		divide_triangle(c, v1, v2, count - 1, color);
		divide_triangle(b, v2, v0, count - 1, color);
		triangle(v0, v2, v1, 4);
	}
	else triangle(a, b, c, color);
}

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4)
{
	i = 0;
	for (int ind = 0; ind < Nvertices; ind++) {
		points[ind] = NULL;
		colors[ind] = NULL;
	}
	divide_triangle(base[3], base[2], base[1], Ndivisions, col2);

	glBindVertexArray(abuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*total, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*total, sizeof(vec3)*total, colors);
}