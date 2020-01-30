#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Angel.h"

#include <iostream>

using namespace std;

#define BUFFER_OFFSET( offset ) ((GLvoid*) (offset))

int NumTimesToSubdivide;
int NumTriangles;
int NumVertices;

vec3* points;
vec3* colors;
int index = 0;
int colorindex;
int axisIndex = 0;
bool refresh = false;

//좌표축별 회전각
enum { stop = 0, Zaxis = 1, Xaxis = 2, Yaxis = 3 };
int Axis = 0;
int currentpt = 0;

float Theta = 0.0;
float scale = 1.0f;

unsigned int width = 720;
unsigned int height = 720;

vec3 baseColors[5] =
{
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.5f, 0.5f, 0.5f)
};
GLuint vao;
GLuint buffer;
GLuint simpleProgram;
GLuint vPosition;
GLuint vColor;

void init();
void display();
void idle();

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli);
void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli);
void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4);

void gasketKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void gasketMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;

	window = glfwCreateWindow(width, height, "practice0507", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); /* Make the window's context current */

	if (glewInit() != GLEW_OK) std::cout << "Error\n";

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	init();

	glfwSetKeyCallback(window, gasketKeyCallback);
	glfwSetMouseButtonCallback(window, gasketMouseButtonCallback);

	///////////////////////////////////////////////////////////////////////////
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		display();
		idle();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	free(points);
	free(colors);
	return 0;
}

void init()
{
	NumTimesToSubdivide = 5;
	NumTriangles = 4 * pow(4, NumTimesToSubdivide);
	NumVertices = 4 * NumTriangles;

	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);

	setGasketVertices(0, 1, 2, 3);

	simpleProgram = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(simpleProgram);

	vPosition = glGetAttribLocation(simpleProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(simpleProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glUseProgram(simpleProgram);

	GLuint uTmp = glGetUniformLocation(simpleProgram, "uTmp");
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
	GLuint uMat = glGetUniformLocation(simpleProgram, "uMat");

	glUniformMatrix4fv(uMat, 1, GL_FALSE, rotation*scaling);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle()
{
	Theta += 0.1f;
}

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli)
{
	points[index] = a;	colors[index++] = baseColors[coli];
	points[index] = b;	colors[index++] = baseColors[coli];
	points[index] = c;	colors[index++] = baseColors[coli];
}

void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli)
{
	if (cnt > 0)
	{
		vec3 ab = (a + b) / 2.0f;
		vec3 ac = (a + c) / 2.0f;
		vec3 bc = (b + c) / 2.0f;
		triangle(ab, bc, ac, 4);
		divide_triangle(a, ab, ac, cnt - 1, coli);
		divide_triangle(c, ac, bc, cnt - 1, coli);
		divide_triangle(b, bc, ab, cnt - 1, coli);
	}
	else
	{
		triangle(a, b, c, coli);
	}
}

void gasketKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case 033:	//Escape key
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 49: case 321:	//1 key
		switch (action) {
		case GLFW_RELEASE:
			cout << "Key 1 Pressed, color changed" << endl;
			setGasketVertices(1, 2, 3, 0);
			currentpt = 1;
			break;
		}
		break;
	case 50: case 322:	//2 key
		switch (action) {
		case GLFW_RELEASE:
			cout << "Key 2 Pressed, color changed" << endl;
			setGasketVertices(2, 3, 0, 1);
			currentpt = 2;
			break;
		}
		break;
	case 51: case 323:	//3 key
		switch (action) {
		case GLFW_RELEASE:
			cout << "Key 3 Pressed, color changed" << endl;
			setGasketVertices(3, 0, 1, 2);
			currentpt = 3;
			break;
		}
		break;
	case 52: case 324:	//4 key
		switch (action) {
		case GLFW_RELEASE:
			cout << "Key 4 Pressed, color changed" << endl;
			setGasketVertices(0, 1, 2, 3);
			currentpt = 0;
			break;
		}
		break;
	case 'u': case'U':
		switch (action) {
		case GLFW_RELEASE:
			refresh = true;
			if (NumTimesToSubdivide < 10) ++NumTimesToSubdivide;
			setGasketVertices(currentpt % 4, (currentpt + 1) % 4, (currentpt + 2) % 4, (currentpt + 3) % 4);
			cout << "Key U pressed, subdivision increased: " << NumTimesToSubdivide << endl;
			break;
		}
		break;
	case 'd': case 'D':
		switch (action) {
		case GLFW_RELEASE:
			refresh = true;
			if (NumTimesToSubdivide > 0) --NumTimesToSubdivide;
			setGasketVertices(currentpt % 4, (currentpt + 1) % 4, (currentpt + 2) % 4, (currentpt + 3) % 4);
			cout << "Key D pressed, subdivision decreased: " << NumTimesToSubdivide << endl;
			break;
		}
		break;
	case 61: case 334://+
		switch (action) {
		case GLFW_RELEASE:
			scale += 0.1f;
			cout << "Key + pressed, scale increased: " << scale << endl;
			break;
		}
		break;
	case 45: case 333:	//-
		switch (action) {
		case GLFW_RELEASE:
			if (scale >= 0.1) scale -= 0.1f;
			cout << "Key - pressed, scale decreased: " << scale << endl;
			break;
		}
		break;
	}
}

void gasketMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			cout << "Mouse left pressed, color changed " << endl;
			currentpt = (++currentpt) % 4;
			setGasketVertices(currentpt % 4, (currentpt + 1) % 4, (currentpt + 2) % 4, (currentpt + 3) % 4);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			cout << "Mouse right pressed, axis changed " << Axis << endl;
			axisIndex = (++axisIndex) % 4;
			Axis = axisIndex;
			break;
		}
	}
}

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4)
{
	index = 0;

	if (refresh) { //정점의 갯수를 늘리려고 했을 때
		free(points);
		free(colors);
		
		NumTriangles = 4 * pow(4, NumTimesToSubdivide);
		NumVertices = 4 * NumTriangles;
		//cout << NumTimesToSubdivide << " " << NumVertices << endl;

		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));
		//refresh 할 때 꼭! 저장되는 정점의 갯수가 버퍼에 맞게 들어갔는지 확인할 것!

		points = (vec3*)malloc(sizeof(vec3)* NumVertices);
		colors = (vec3*)malloc(sizeof(vec3)* NumVertices);

		refresh = false;
	}
	
	vec3 base[4] =
	{
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.942809f, -0.333333f),
		vec3(-0.816497f, -0.471405f, -0.333333f),
		vec3(0.816497f, -0.471405f, -0.333333f)
	};

	divide_triangle(base[0], base[1], base[2], NumTimesToSubdivide, col1);
	divide_triangle(base[3], base[2], base[1], NumTimesToSubdivide, col2);
	divide_triangle(base[0], base[3], base[1], NumTimesToSubdivide, col3);
	divide_triangle(base[0], base[2], base[3], NumTimesToSubdivide, col4);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
}
