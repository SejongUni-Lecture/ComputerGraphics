//3_3 다양한 색상의 정사각형 그리기(선) 
//Tip!: 선 하나하나 객체로 간주해야 한다.

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<Angel.h>
#include<InitShader.h>
#define BUFFER_OFFSET( offset ) ((GLvoid *) (offset))

using namespace std;
unsigned int width = 512;
unsigned int height = 512;

vec3 *points;
vec3 *colors;

float Theta = 0.0;
int NumVertices;
int NumIndices;
int Index = 0;
float scale = 1.0f;

vec3 baseColors[5] = {
	vec3(1.0f,0.0f,0.0f),
	vec3(0.0f,1.0f,0.0f),
	vec3(0.0f,0.0f,1.0f),
	vec3(0.7f,0.0f,0.7f),
	vec3(0.0f,0.0f,0.0f)
};

GLuint vao;
GLuint buffer;
GLuint program;
GLuint vPosition;
GLuint vColor;

void drawRectangle(vec2 *Indices);
void triangle(vec3 a, vec3 b, unsigned int coli);
void Init();
void display();
void idle();

int main() {
	GLFWwindow *window;
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(width, height, "Assignment3", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) cout << "Error" << endl;

	Init();

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

void Init() {

	vec2 Indices[4] = {
		vec2(0,1),
		vec2(1,2),
		vec2(2,3),
		vec2(3,0),
	};

	NumVertices = 40;	//5개 정사각형에서 4번 선으로 그리되, 2개의 점을 잇는다.
	NumIndices = 4;

	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);

	drawRectangle(Indices);

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));

	//glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glUseProgram(program);

	mat4 rotation = RotateY(Theta);
	mat4 scaling = Scale(0.5f);
	GLuint uMat = glGetUniformLocation(program, "uMat");
	glUniformMatrix4fv(uMat, 1, GL_FALSE, scaling);

	glDrawArrays(GL_LINES, 0, NumVertices);
}

void idle() {
	Theta += 0.5f;
}

void drawRectangle(vec2 *Indices) {

	vec3 vertices[4] = {
		vec3(-1.0f,1.0f,0.0f),
		vec3(-1.0f,-1.0f,0.0f),
		vec3(1.0f,-1.0f,0.0f),
		vec3(1.0f,1.0f,0.0f),
	};

	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < NumIndices; i++)
		{
			int ia = Indices[i].x;
			int ib = Indices[i].y;

			triangle(vertices[ia], vertices[ib], j);
		}
		scale -= 0.1;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
}

void triangle(vec3 a, vec3 b, unsigned int coli)
{
	points[Index] = a * scale;	colors[Index++] = baseColors[coli];
	points[Index] = b * scale;	colors[Index++] = baseColors[coli];
}