#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

struct MyCubeVertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec3 normal;	//+ normal Á¤º¸
};

class MyCube
{
public:
	MyCube(void);
	~MyCube(void);

	static const int NumVertices = 36;

	MyCubeVertex Vertices[NumVertices];
	GLuint Init();
	void SetPositionAndColorAttribute(GLuint program);


	void ColorCube(glm::vec4 * vin, glm::vec4 * cin);
	void Quad(int a, int b, int c, int d, glm::vec4 * vin, glm::vec4 * cin);

	int NumCurVertices;

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};