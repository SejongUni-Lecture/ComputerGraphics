#include "Angel.h"
#include<iostream>

using namespace std;

#define BUFFER_OFFSET(offset) ((GLvoid*)(offset))

unsigned int width = 512;
unsigned int height = 512;

GLuint vao;
GLuint buffer;
GLuint simpleShader;

unsigned int NumFaces;
unsigned int NumTriangles;
unsigned int NumVertices;

vec3 *points;
vec3 *colors;
int index = 0;
float theta = 0.0f;
float scale = 0.1f;
float scaleUnit = 0.0f;

vec3 baseVertices[] = {
	vec3(-0.5f,-0.5f,-0.5f),
	vec3(0.5f,-0.5f,-0.5f),
	vec3(0.5f,0.5f,-0.5f),
	vec3(-0.5f,0.5f,-0.5f),

	vec3(-0.6f,-0.6f,0.6f),
	vec3(0.6f,-0.6f,0.6f),
	vec3(0.6f,0.6f,0.6f),
	vec3(-0.6f,0.6f,0.6f)
};

vec3 baseColors[] = {
	vec3(1.0f,0.0f,0.0f),		//red
	vec3(0.0f,1.0f,0.0f),		//green
	vec3(0.0f,0.0f,1.0f),		//blue
	vec3(1.0f,1.0f,0.0f),		//yellow	
	vec3(1.0f,0.0f,1.0f),		//magenta
	vec3(0.0f,1.0f,1.0f),		//cyan
	vec3(0.5f,0.5f,0.5f),		//grey
	vec3(1.0f,1.0f,1.0f),		//white
};

void init();
void display();
void idle();

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli);
void quad(vec3 a, vec3 b, vec3 c, vec3 d, unsigned int coli);
void cube(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4, unsigned int col5, unsigned int col6);

int main() {
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(width, height, "practice0509", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) cout << "ERROR" << endl;

	init();

	while (!glfwWindowShouldClose(window)) {
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

void init() {
	NumFaces = 6;
	NumTriangles = 2 * NumFaces;
	NumVertices = 3 * NumTriangles;

	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	cube(0, 1, 2, 3, 4, 5);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);

	simpleShader = InitShader("vshader36.glsl", "fshader36.glsl");
	glUseProgram(simpleShader);

	GLuint vPosition = glGetAttribLocation(simpleShader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	GLuint vColor = glGetAttribLocation(simpleShader, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUseProgram(simpleShader);

	GLuint uTmp = glGetUniformLocation(simpleShader, "uTmp");
	glUniform1i(uTmp, 1);

	mat4 rotation = RotateY(theta);
	mat4 scaling = Scale(scale);
	GLuint uMat = glGetUniformLocation(simpleShader, "uMat");
	glUniformMatrix4fv(uMat, 1, GL_FALSE, rotation*scaling);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle() {

	theta += 0.5f;
	scale += scaleUnit;
	if (scale <= 0.01f || scale >= 0.5f) scaleUnit += -1.0f;
}

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli) {
	points[index] = a;	colors[index++] = baseColors[coli];
	points[index] = b;	colors[index++] = baseColors[coli];
	points[index] = c;	colors[index++] = baseColors[coli];
}

void quad(vec3 a, vec3 b, vec3 c, vec3 d, unsigned int coli) {
	triangle(a, b, c, coli);
	triangle(a, c, d, coli);
}

void cube(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4, unsigned int col5, unsigned int col6) {
	quad(baseVertices[0], baseVertices[1], baseVertices[2], baseVertices[3], col1);	//red
	quad(baseVertices[1], baseVertices[5], baseVertices[6], baseVertices[2], col2);	//green
	quad(baseVertices[4], baseVertices[0], baseVertices[3], baseVertices[7], col4);	//yellow	
	quad(baseVertices[3], baseVertices[2], baseVertices[6], baseVertices[7], col5);	//magenta
	quad(baseVertices[4], baseVertices[5], baseVertices[1], baseVertices[0], col6);	//cyan
	quad(baseVertices[5], baseVertices[4], baseVertices[7], baseVertices[6], col3);	//blue
}