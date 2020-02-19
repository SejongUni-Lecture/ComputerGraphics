#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

struct MySphereVertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec3 normal;
};

class MySphere
{
public:
	MySphere(void);
	~MySphere(void);

	int NumVertices;

	MySphereVertex * Vertices;
	GLuint Init(int la_slice, int lo_slice, glm::vec4 color = glm::vec4(0.5, 0.5, 0.5, 1));
	void SetPositionAndOtherAttributes(GLuint program);

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};