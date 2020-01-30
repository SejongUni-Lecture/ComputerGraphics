#include "Angel.h"
#include<iostream>

using namespace std;

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

const int NumVerticies = 36;			//for array, const

point4 points[NumVerticies];
color4 colors[NumVerticies];

//������ �߽�����, �࿡ ������ ���� ������ü�� ������
point4 vertices[8] = {
	point4(-0.5,-0.5,0.5,1.0),
	point4(-0.5,0.5,0.5,1.0),
	point4(0.5,0.5,0.5,1.0),
	point4(0.5,-0.5,0.5,1.0),

	point4(-0.5,-0.5,-0.5,1.0),
	point4(-0.5,0.5,-0.5,1.0),
	point4(0.5,0.5,-0.5,1.0),
	point4(0.5,-0.5,-0.5,1.0)
};

//RGBA �����

color4 vertex_colors[8] = {
	color4(0.0,0.0,0.0,1.0),	//black
	color4(1.0,0.0,0.0,1.0),	//red
	color4(1.0,1.0,0.0,1.0),	//yellow
	color4(0.0,1.0,0.0,1.0),	//green
	color4(0.0,0.0,1.0,1.0),	//blue
	color4(1.0,0.0,1.0,1.0),	//magenta
	color4(1.0,1.0,1.0,1.0),	//white
	color4(0.0,1.0,1.0,1.0),	//cyan
};

//��ǥ�ະ ȸ����
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxis = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxis] = { 0.0,0.0,0.0 };
GLuint theta; //���̴� ������ ������ ��ġ ����

int index = 0;

//�� �鿡 ���ؼ� �ﰢ�� 2���� �簢���� ����ϰ�, �������� ���� �ο�
void quad(int a, int b, int c, int d) {
	colors[index] = vertex_colors[a];	points[index] = vertices[a];	index++;
	colors[index] = vertex_colors[b];	points[index] = vertices[b];	index++;
	colors[index] = vertex_colors[c];	points[index] = vertices[c];	index++;
	colors[index] = vertex_colors[a];	points[index] = vertices[a];	index++;
	colors[index] = vertex_colors[c];	points[index] = vertices[c];	index++;
	colors[index] = vertex_colors[d];	points[index] = vertices[d];	index++;
}

//36���� �������� 36���� ������ �ڱ⵵�� 12���� �ﰢ�� ���
void colorcube(void) {
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);		//**������ ��ǥ�迡 ���� normal(face front)�� �����ؼ� �����ϵ��� �Ѵ�.
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(5, 4, 0, 1);
	quad(4, 5, 6, 7);
}

void init(void) {
	colorcube();

	//���̴� �ҽ��� �о ���̴� ���α׷� ���
	GLuint program = InitShader("vshader36.glsl", "fshader36.glsl");
	glUseProgram(program);

	//���� �迭 ��ü ����
	GLuint vao;
	glGenVertexArrays(1, &vao);	//**��ü ��ó ����
	glBindVertexArray(vao);		//**��ü ���� ���

	//���� ��ü ����
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);	//variable: target, size, data(pt), usage
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);	//variable: target, offset, size, data(pt)
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	//���� �迭 ����
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
	
	theta = glGetUniformLocation(program, "theta");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1, 0.1, 0.1, 1.0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_TRIANGLES, 0, NumVerticies);
}

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
	case 033:	//Escape key
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

void mouse(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			Axis = Xaxis; break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			Axis = Yaxis; break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			Axis = Zaxis; break;
		}
	}
}

void idle(void) {
	Theta[Axis] += 0.1;
	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}
}

int main() {

	GLFWwindow* window;
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(720, 720, "CUBE", NULL, NULL);
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
		glfwSetKeyCallback(window, keyboard);
		glfwSetMouseButtonCallback(window, mouse);
		//glfwSetWindowSizeCallback(idle);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}
