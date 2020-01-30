//3_5 키보드 입력에 따른 도형 출력하기
//과제2에서 했던 refresh 방법과 동일 

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

int NumVertices;
int NumIndices;
int Index = 0;
int Index2 = 0;
bool squareQuick = false;
bool triQuick = false;
bool squareSlow = false;
bool triSlow = false;
float scale = 1.0f;
float sSpeed = 0.5f;
float tSpeed = 0.1f;
float sTheta = 0.0f;
float tTheta = 0.0f;

vec3 baseColors[1] = {
	vec3(0.7f,0.0f,0.7f),
};

vec3 SqIndices[2] = {
	vec3(3,0,1),
	vec3(1,2,3)
};

vec3 TriIndices[1] = {
	vec3(0,1,2)
};

GLuint vao;
GLuint buffer;
GLuint program;
GLuint vPosition;
GLuint vColor;

void drawRectangle(vec3 *Indices);
void drawTriangle(vec3 *Indices);
void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli);
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
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

	glfwSetKeyCallback(window, KeyCallBack);

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

	NumVertices = 30;
	NumIndices = 2;

	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);

	drawRectangle(SqIndices);

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

	mat4 rotation = RotateZ(sTheta);
	mat4 scaling = Scale(0.5f);
	GLuint uMat = glGetUniformLocation(program, "uMat");
	glUniformMatrix4fv(uMat, 1, GL_FALSE, rotation*scaling);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle() {
	if (squareQuick) {
		sSpeed += 0.1;
		squareQuick = false;
	}
	else if (squareSlow) {
		sSpeed -= 0.1f;
		squareSlow = false;
	}
	else if (triQuick) {
		tSpeed = 0.5f;
	}
	else {
		tSpeed = 0.1f;
	}
	sTheta += sSpeed;
}

void drawRectangle(vec3 *Indices) {

	Index = 0;

	free(points);
	free(colors);

	NumVertices = 6;
	NumIndices = 2;

	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));
	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	vec3 vertices[4] = {
		vec3(-1.0f,1.0f,0.0f),
		vec3(-1.0f,-1.0f,0.0f),
		vec3(1.0f,-1.0f,0.0f),
		vec3(1.0f,1.0f,0.0f),
	};

	for (int i = 0; i < NumIndices; i++)
	{
		int ia = Indices[i].x;
		int ib = Indices[i].y;
		int ic = Indices[i].z;
		
		triangle(vertices[ia], vertices[ib], vertices[ic], 0);
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
}

void drawTriangle(vec3 *Indices) {
	
	Index = 0;

	free(points);
	free(colors);

	NumVertices = 3;
	NumIndices = 1;

	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));
	
	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);


	vec3 vertices[3] = {
		vec3(0.0f,1.0f,0.0f),
		vec3(-1.0f,-1.0f,0.0f),
		vec3(1.0f,-1.0f,0.0f),
	};

	for (int i = 0; i < NumIndices; i++) {
		int ia = Indices[i].x;
		int ib = Indices[i].y;
		int ic = Indices[i].z;

		triangle(vertices[ia], vertices[ib], vertices[ic], 0);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
}

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli)
{
	points[Index] = a * scale;	colors[Index++] = baseColors[coli];
	points[Index] = b * scale;	colors[Index++] = baseColors[coli];
	points[Index] = c * scale;	colors[Index++] = baseColors[coli];
}
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case 'q': case 'Q':
		switch (action) {
		case GLFW_RELEASE:
			squareQuick = true;
			squareSlow = false;
			break;
		}
		break;
	case 'w': case'W':
		switch (action) {
		case GLFW_RELEASE:
			squareSlow = true;
			squareQuick = false;
			break;
		}
		break;
	case 'e': case'E':
		switch (action) {
		case GLFW_RELEASE:
			triSlow = false;
			triQuick = true;
			break;
		}
	case 'r': case'R':
		switch (action) {
		case GLFW_RELEASE:
			triSlow = true;
			triQuick = false;
			break;
		}
		break;
	}
}
