#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <InitShader.h>
#include <CheckError.h>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "myModel.h"
#include "Angel.h"

using namespace std;

float time = 0.0f;

unsigned int width = 512;
unsigned int height = 512;

GLuint phongShader;

MyModel model;

//좌표축별 회전각
enum { stop = 0, Zaxis = 1, Xaxis = 2, Yaxis = 3 };
int Axis = 0;
int axisIndex = 0;
bool PAUSE_PROGRAM = false;
int cnt = 0;

GLfloat Theta[3] = { 0.0,0.0,0.0 };
GLuint theta;
float scale = 1.0f;
char path[11] = "";

void init();
void display();
void idle();
void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;

	window = glfwCreateWindow(width, height, "Simple Obj Viewer", NULL, NULL);
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
	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	///////////////////////////////////////////////////////////////////////////
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, width, height);

		display();
		idle();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void init()
{
	phongShader = InitShader("vPhongShading.glsl", "fPhongShading.glsl");	// for phong shading
	
	cout << "Input Object Path: ";
	cin >> path;
	//strcpy(path, "dragon.obj");

	//cout << "Model Initial" << endl;
	model.Init(path);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

void display()
{
	//cout << "Start display" << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint myShader = phongShader;
	glUseProgram(myShader);

	// camera view matrix
	glm::mat4 myView = glm::mat4(1.0f);
	myView = glm::lookAt(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//myView = glm::lookAt(glm::vec3(2.0f*cos(time), 2.0f, -2.0f*sin(time)), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	// orthogonal projection matrix
	glm::mat4 myOrtho = glm::mat4(1.0f);
	myOrtho = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
	//myOrtho = glm::ortho(-5.0f*sin(time), 5.0f*sin(time), -5.0f*sin(time), 5.0f*sin(time), -5.0f*sin(time), 5.0f*sin(time));

	// perspective projection matrix
	glm::mat4 myPerspective = glm::mat4(1.0f);
	myPerspective = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 10.0f);
	//myPerspective = glm::perspective(glm::radians(30.0f*sin(time) + 30.0f), 1.0f, 0.01f, 10.0f);
	 
		// for material properties
	glm::vec4 ma = glm::vec4(0.1, 0.4, 0.1, 1);	// ambient
	glm::vec4 md = glm::vec4(0.8, 0.8, 0.8, 1);		// diffse
	glm::vec4 ms = glm::vec4(1, 1, 1, 1);		// specular

	// for light properties
	glm::vec4 ld = glm::vec4(0.1, 0.3, 0.2, 1);		// diffuse
	glm::vec4 la = glm::vec4(0.3, 0.5, 0.3, 1);		// ambient light

	// for location
	//glm::vec4 ll = glm::vec4(0, 10, 0, 1);		// light
	glm::vec4 locLight = glm::vec4(10.0f*sin(time), 1.0f, 10.0f*cos(time), 1.0f);		// light
	//glm::vec4 locLight = glm::vec4(0, -10, 0, 1);		
	glm::vec4 locEye = glm::vec4(0, 0, 0, 1);		// eye

	int gloss = 50;

	glm::vec4 amb = la * ma;
	glm::vec4 dif = ld * md;
	glm::vec4 spc = ms;

	// for camera view and projection
	GLuint uModelView = glGetUniformLocation(myShader, "uModel");
	GLuint uOrtho = glGetUniformLocation(myShader, "uOrtho");
	GLuint uPerspective = glGetUniformLocation(myShader, "uPerspective");
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &myView[0][0]);
	glUniformMatrix4fv(uOrtho, 1, GL_FALSE, &myOrtho[0][0]);
	glUniformMatrix4fv(uPerspective, 1, GL_FALSE, &myPerspective[0][0]);

	// for shading
	GLuint uAmb = glGetUniformLocation(myShader, "uAmb");
	GLuint uDif = glGetUniformLocation(myShader, "uDif");
	GLuint uSpc = glGetUniformLocation(myShader, "uSpc");
	GLuint uMAmb = glGetUniformLocation(myShader, "uMAmb");
	GLuint uMDif = glGetUniformLocation(myShader, "uMDif");
	GLuint uMSpec = glGetUniformLocation(myShader, "uMSpec");
	GLuint uLAmb = glGetUniformLocation(myShader, "uLAmb");
	GLuint uLDif = glGetUniformLocation(myShader, "uLDif");
	GLuint uLocLgt = glGetUniformLocation(myShader, "uLocLgt");
	GLuint uLocEye = glGetUniformLocation(myShader, "uLocEye");
	GLuint uGloss = glGetUniformLocation(myShader, "uGloss");

	glUniform4f(uAmb, amb[0], amb[1], amb[2], amb[3]);
	glUniform4f(uDif, dif[0], dif[1], dif[2], dif[3]);
	glUniform4f(uSpc, spc[0], spc[1], spc[2], spc[3]);
	glUniform4fv(uMAmb, 1, &ma[0]);
	glUniform4fv(uMDif, 1, &md[0]);
	glUniform4fv(uMSpec, 1, &ms[0]);
	glUniform4fv(uLAmb, 1, &la[0]);
	glUniform4fv(uLDif, 1, &ld[0]);
	glUniform4fv(uLocLgt, 1, &locLight[0]);
	glUniform4fv(uLocEye, 1, &locEye[0]);
	glUniform1i(uGloss, gloss);

	mat4 rotation = RotateX(Theta[0])* RotateY(Theta[1])*RotateZ(Theta[2]);
	//mat4 translateCenter = Translate(-model.center.x, -model.center.y, -model.center.z);
	mat4 CTM =Scale(model.Scale) * rotation;
	GLuint uMat = glGetUniformLocation(myShader, "uMat");
	glUniformMatrix4fv(uMat, 1, GL_FALSE, CTM);

	model.Draw(myShader);
}

void idle() {
	if (!PAUSE_PROGRAM) {
		Theta[Axis] += 0.1f;

		if (Theta[Axis] > 360.0) {
			Theta[Axis] -= 360.0;
		}
	}
	time += 0.002f;
}

void KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_SPACE:
		switch (action) {
		case GLFW_RELEASE:
			cout << "SPACE BAR PRESSED" << endl;
			if (cnt % 2 == 0)PAUSE_PROGRAM = true;
			else PAUSE_PROGRAM = false;
			++cnt;
		}
	}
}

void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			cout << "MOUSE BUTTON LEFT PRESSED" << endl;
			axisIndex = (++axisIndex) % 3;
			Axis = axisIndex;
			break;
		}
	}
}