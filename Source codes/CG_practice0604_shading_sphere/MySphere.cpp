#pragma once

#include "MySphere.h"

MySphere::MySphere(void)
{
	bInitialized = false;
	NumVertices = 0;
	Vertices = NULL;
}

MySphere::~MySphere(void)
{
	if (Vertices != NULL)
		delete[] Vertices;
}

GLuint MySphere::Init(int la_slice, int lo_slice, glm::vec4 color)
{
	// The Cube should be initialized only once;
	if (bInitialized == true) return vao;

	NumVertices = (la_slice - 2)*lo_slice * 2 * 3 + 2 * lo_slice * 3;
	Vertices = new MySphereVertex[NumVertices];

	float da = 3.141592 / la_slice;
	float db = 3.141592*2.0 / lo_slice;
	int cur = 0;
	for (int j = 0; j < lo_slice; j++)
	{
		float y2 = cos(da);
		float r2 = sqrt(1 - y2 * y2);
	}
	for (int i = 0; i < la_slice; i++)
	{
		float y1 = cos(da*i);
		float r1 = sqrt(1 - y1 * y1);
		float y2 = cos(da*(i + 1));
		float r2 = sqrt(1 - y2 * y2);
		for (int j = 0; j < lo_slice; j++)
		{
			glm::vec3 a(r1*cos(db*j), y1, r1*sin(db*j));
			glm::vec3 b(r2*cos(db*j), y2, r2*sin(db*j));
			glm::vec3 c(r2*cos(db*(j + 1)), y2, r2*sin(db*(j + 1)));
			glm::vec3 d(r1*cos(db*(j + 1)), y1, r1*sin(db*(j + 1)));

			if (i != lo_slice - 1)
			{
				Vertices[cur].position = glm::vec4(a, 1.0f);	Vertices[cur].color = color; Vertices[cur].normal = a; cur++;
				Vertices[cur].position = glm::vec4(b, 1.0f);	Vertices[cur].color = color; Vertices[cur].normal = b; cur++;
				Vertices[cur].position = glm::vec4(c, 1.0f);	Vertices[cur].color = color; Vertices[cur].normal = c; cur++;
			}
			if (i != 0)
			{
				Vertices[cur].position = glm::vec4(c, 1.0f);	Vertices[cur].color = color; Vertices[cur].normal = c; cur++;
				Vertices[cur].position = glm::vec4(d, 1.0f);	Vertices[cur].color = color; Vertices[cur].normal = d; cur++;
				Vertices[cur].position = glm::vec4(a, 1.0f);	Vertices[cur].color = color; Vertices[cur].normal = a; cur++;
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MySphereVertex)*NumVertices, Vertices, GL_STATIC_DRAW);

	bInitialized = true;
	return vao;
}

void MySphere::SetPositionAndOtherAttributes(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MySphereVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MySphereVertex), BUFFER_OFFSET(sizeof(glm::vec4)));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, sizeof(MySphereVertex), BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::vec4)));

}


void MySphere::Draw(GLuint program)
{
	if (!bInitialized) return;			// check whether it is initiazed or not. 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndOtherAttributes(program);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}