#include "MyCube.h"

MyCube::MyCube(void)
{
	bInitialized = false;
	NumCurVertices = 0;
}

MyCube::~MyCube(void)
{
}

void MyCube::Quad(int a, int b, int c, int d, glm::vec4 * vin, glm::vec4 * cin)
{
	//+ �ﰢ���� ��� ���� ����ϱ�

	glm::vec4 p4, q4;	//+ �ﰢ���� �� ���� ���� ����(xyzw)
	glm::vec3 p, q;		//+ �ﰢ���� �� ���� ���� ����(xyz)
	glm::vec3 n;		//+ normal ����

	//+ quad�� ù��° �ﰢ��
	p4 = vin[b] - vin[a];	//+ ba ����
	q4 = vin[c] - vin[a];	//+ ca ����

	p = glm::vec3(p4.x, p4.y, p4.z);	//+ ba ������ xyz
	q = glm::vec3(q4.x, q4.y, q4.z);	//+ ca ������ xyz

	n = cross(p, q);	//+ �ﰢ���� �� ���͸� cross product �ϸ� �ﰢ���� ������ ���͸� ���� �� �ִ�
	n = normalize(n);	//+ �ﰢ���� ������ ���͸� normalization

	Vertices[NumCurVertices].position = vin[a]; Vertices[NumCurVertices].color = cin[a];
	Vertices[NumCurVertices].normal = n;		Vertices[NumCurVertices].texcoord = glm::vec2(0, 0);
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[b]; Vertices[NumCurVertices].color = cin[b];
	Vertices[NumCurVertices].normal = n;		Vertices[NumCurVertices].texcoord = glm::vec2(1, 0);
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[c]; Vertices[NumCurVertices].color = cin[c];
	Vertices[NumCurVertices].normal = n;		Vertices[NumCurVertices].texcoord = glm::vec2(1, 1);
	NumCurVertices++;

	//+ quad�� �ι�° �ﰢ��
	p4 = vin[c] - vin[a];	//+ ca ����
	q4 = vin[d] - vin[a];	//+ da ����

	p = glm::vec3(p4.x, p4.y, p4.z);	//+ ca ������ xyz
	q = glm::vec3(q4.x, q4.y, q4.z);	//+ da ������ xyz

	n = cross(p, q);	//+ �ﰢ���� �� ���͸� cross product �ϸ� �ﰢ���� ������ ���͸� ���� �� �ִ�
	n = normalize(n);	//+ �ﰢ���� ������ ���͸� normalization

	Vertices[NumCurVertices].position = vin[c]; Vertices[NumCurVertices].color = cin[c];
	Vertices[NumCurVertices].normal = n;		Vertices[NumCurVertices].texcoord = glm::vec2(1, 1);
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[d]; Vertices[NumCurVertices].color = cin[d];
	Vertices[NumCurVertices].normal = n;		Vertices[NumCurVertices].texcoord = glm::vec2(0, 1);
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[a]; Vertices[NumCurVertices].color = cin[a];
	Vertices[NumCurVertices].normal = n;		Vertices[NumCurVertices].texcoord = glm::vec2(0, 0);
	NumCurVertices++;

}

void MyCube::ColorCube(glm::vec4 * vin, glm::vec4 * cin)
{
	Quad(0, 1, 2, 3, vin, cin);
	Quad(1, 5, 6, 2, vin, cin);
	Quad(5, 4, 7, 6, vin, cin);
	Quad(4, 0, 3, 7, vin, cin);
	Quad(3, 2, 6, 7, vin, cin);
	Quad(4, 5, 1, 0, vin, cin);
}


GLuint MyCube::Init()
{
	// The Cube should be initialized only once;
	if (bInitialized == true) return vao;

	glm::vec4 vertex_positions[8] = {
		glm::vec4(-0.5, -0.5, -0.5, 1.0),
		glm::vec4(0.5,  -0.5, -0.5, 1.0),
		glm::vec4(0.5,  0.5, -0.5, 1.0),
		glm::vec4(-0.5, 0.5, -0.5, 1.0),

		glm::vec4(-0.5, -0.5, 0.5, 1.0),
		glm::vec4(0.5,  -0.5, 0.5, 1.0),
		glm::vec4(0.5,  0.5, 0.5, 1.0),
		glm::vec4(-0.5, 0.5, 0.5, 1.0),
	};

	glm::vec4 vertex_colors[8] = {
		glm::vec4(0, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 1, 1, 1),
		glm::vec4(0, 1, 0, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(1, 0, 1, 1),
		glm::vec4(1, 1, 0, 1),
		glm::vec4(0, 1, 1, 1)
	};

	ColorCube(vertex_positions, vertex_colors);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	bInitialized = true;
	return vao;
}

void MyCube::SetPositionAndColorAttribute(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(glm::vec4)));

	//+ normal property setting
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::vec4)));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(glm::vec3)));
}


void MyCube::Draw(GLuint program)
{
	if (!bInitialized) return;			// check whether it is initiazed or not. 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndColorAttribute(program);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}