//#include<iostream>
//#include<stdlib.h>
//#include"Angel.h"
//#include"indi.h"
//
///*********************************************/
////	 3D Gasket programming using tetra
////	        github@ameliacode
///*********************************************/
//
//using namespace Angel;
//using namespace std;
//
//typedef vec3 point3;
//typedef vec3 color3;
//
//const int Ndivisions = 4;
//const int Nvertices = 5000;	
//
//int i = 0;
//int colorindex;
//vec3 points[Nvertices];
//vec3 colors[Nvertices];
//vec3 base_color[4] = { vec3(0.0,0.0,0.0), vec3(0.0,1.0,0.0), 
//						 vec3(0.0,0.0,1.0), vec3(1.0,0.0,0.0) };
//
////function lists
//void init(void);
//void triangle(const vec3& a, const vec3& b, const vec3& c);
//void tetra(const vec3& a, const vec3& b, const vec3& c, const vec3& d);
//void divide_tetra(const vec3& a, const vec3& b, const vec3& c, const vec3& d, int m);
//
//int main() {
//
//	GLFWwindow* window;
//	if (!glfwInit()) return -1;
//
//	window = glfwCreateWindow(512, 512, "Gasket", NULL, NULL);
//	if (!window) {
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	if (glewInit() != GLEW_OK) std::cout << "Error" << endl;
//	
//	init();
//	while (!glfwWindowShouldClose(window)) {
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glDrawArrays(GL_TRIANGLES, 0, Nvertices);
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	glfwTerminate();
//
//	return 0;
//}
//
//
//void init(void) {
//
//	//vec3 vertices[4] = { {0.0,0.0,1.0},{0.0,0.942809,-0.33333},
//	//					{-0.816497,-0.471405,-0.333333}, {0.816497,-0.471405,-0.333333} };
//	vec3 vertices[4] = { vec3(-1.0,-1.0,-1.0), vec3(1.0,-1.0,-1.0), vec3(0.0,1.0,-1.0), vec3(0.0,0.0,1.0) };
//
//	std::cout << "let's divide" << endl;
//	divide_tetra(vertices[0], vertices[1], vertices[2], vertices[3], Ndivisions);
//	//tetra(vertices[0], vertices[1], vertices[2], vertices[3]);
//	//triangle(vertices[0], vertices[1], vertices[3]);
//
//	std::cout << "create vBuffer" << endl;
//	GLuint abuffer;					//vertex buffer
//	glGenVertexArrays(1, &abuffer);
//	glBindVertexArray(abuffer);
//
//	std::cout << "create buffer object" << endl;
//	GLuint buffer;					//buffer object
//	glGenBuffers(1, &buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
//	
//	std::cout << "initialize shader" << endl;
//	GLuint program = InitShader("vsource.glsl", "fsource.glsl");
//	glUseProgram(program);
//
//	GLuint loc;
//	loc = glGetAttribLocation(program, "vPosition");
//	glEnableVertexAttribArray(loc);
//	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//
//	GLuint loc2 = glGetAttribLocation(program, "vColor");
//	glEnableVertexAttribArray(loc2);
//	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
//	
//	//glEnable(GL_DEPTH_TEST);
//	glClearColor(1.0, 1.0, 1.0, 1.0);	//white background
//}
//
//void triangle(const vec3& a, const vec3& b, const vec3& c) {
//	colors[i] = base_color[colorindex];
//	points[i] = a;
//	++i;
//	colors[i] = base_color[colorindex];
//	points[i] = b;
//	++i;
//	colors[i] = base_color[colorindex];
//	points[i] = c;
//	++i;
//}
//
//void tetra(const vec3& a, const vec3& b, const vec3& c, const vec3& d) {
//	colorindex = 0;
//	triangle(a, b, c);
//	colorindex = 1;
//	triangle(a, c, d);
//	colorindex = 2;
//	triangle(a, d, b);
//	colorindex = 3;
//	triangle(b, d, c);
//}
//
//void divide_tetra(const vec3& a, const vec3& b, const vec3& c, const vec3& d, int m) {
//	int j;
//	if (m > 0) {
//		vec3 mid[6];
//
//		mid[0] = (a + b) / 2.0;
//		mid[1] = (a + c) / 2.0;
//		mid[2] = (a + d) / 2.0;
//		mid[3] = (b + c) / 2.0;
//		mid[4] = (c + d) / 2.0;
//		mid[5] = (b + d) / 2.0;
//
//		divide_tetra(a, mid[0], mid[1], mid[2], m - 1);
//		divide_tetra(mid[0], b, mid[3], mid[5], m - 1);
//		divide_tetra(mid[1], mid[3], c, mid[4], m - 1);
//		divide_tetra(mid[2], mid[5], mid[4], d, m - 1);
//	}
//	else tetra(a, b, c, d);
//}