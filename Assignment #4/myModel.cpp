#include "myModel.h"
#include <iostream>
using namespace std;

MyModel::MyModel(void)
{
	NumFaces = 0;
	NumVertices = 0;
	NumCurVertices = 0;
	vertexCnt = 0;
	bInitialized = false;
	Scale = 1;
	center = glm::vec3(0.0, 0.0, 0.0);
}

MyModel::~MyModel(void)
{
	free(Vertices);
	free(Indices);
}

void MyModel:: Tri(float a, float b, float c, glm::vec4 * vin, glm::vec4 * cin)
{
	//+ 삼각형의 노멀 벡터 계산하기

	glm::vec4 p4, q4;	//+ 삼각형의 두 변에 대한 벡터(xyzw)
	glm::vec3 p, q;		//+ 삼각형의 두 변에 대한 벡터(xyz)
	glm::vec3 n;		//+ normal 벡터

	int ia = (int)a;
	int ib = (int)b;
	int ic = (int)c;

	//+ quad의 첫번째 삼각형
	p4 = vin[ib] - vin[ia];	//+ ba 벡터
	q4 = vin[ic] - vin[ia];	//+ ca 벡터

	p = glm::vec3(p4.x, p4.y, p4.z);	//+ ba 벡터의 xyz
	q = glm::vec3(q4.x, q4.y, q4.z);	//+ ca 벡터의 xyz

	//면에 대한 normal을 구하는 것이라서 flat shading이 형성된다...
	//phong shading을 하고 싶으면, interpolate를 통해서 버텍스별 노멀을 구해야 한다.

	n = cross(p, q);	//+ 삼각형의 두 벡터를 cross product 하면 삼각형에 수직인 벡터를 구할 수 있다
	n = normalize(n);	//+ 삼각형에 수직인 벡터를 normalization

	Vertices[NumCurVertices].position = vin[ia] - glm::vec4(center.x, center.y, center.z, 0.0);
	Vertices[NumCurVertices].color = cin[0]; 
	Vertices[NumCurVertices].normal = n;	//flat shading 
	//Vertices[NumCurVertices].normal = Vertices[NumCurVertices].position;	//flat shading 
	//Vertices[NumCurVertices].normal = glm::vec3(0.0, 0.0, 0.0);
	//Vertices[ia].normal += n;
	NumCurVertices++;

	Vertices[NumCurVertices].position = vin[ib] - glm::vec4(center.x, center.y, center.z, 0.0);
	Vertices[NumCurVertices].color = cin[0]; 
	Vertices[NumCurVertices].normal = n;
	//Vertices[NumCurVertices].normal = Vertices[NumCurVertices].position;
	//Vertices[NumCurVertices].normal = glm::vec3(0.0, 0.0, 0.0);;
	//Vertices[ib].normal += n;
	NumCurVertices++;

	Vertices[NumCurVertices].position = vin[ic] - glm::vec4(center.x, center.y, center.z, 0.0);
	Vertices[NumCurVertices].color = cin[0]; 
	Vertices[NumCurVertices].normal = n;
	//Vertices[NumCurVertices].normal = Vertices[NumCurVertices].position;
	//Vertices[NumCurVertices].normal = glm::vec3(0.0, 0.0, 0.0);;
	//Vertices[ic].normal += n;
	NumCurVertices++;
}

void MyModel::ShadeModel(glm::vec4 * vin, glm::vec4 *cin)
{
	for (int i = 0; i < NumFaces; i++) {
		Tri(Indices[i].x, Indices[i].y, Indices[i].z, vin, cin);
	}
	//For phong shading 
	//CalculateVertexNormal();
}

void MyModel::CalculateVertexNormal() {
	
	//for phong shading, we need to calculate additionally 
	//else, it will result as "flat shading"

	//Vert     *vert = myself->vert;
	//Triangle *face = myself->face;

	for (int i = 0; i < vertexCnt + 1; i++) 
		Vertices[i].normal =glm::vec3(0.0f);

	for (int i = 0; i < NumFaces; i++)
	{
		int ia = Indices[i].x;
		int ib = Indices[i].y;
		int ic = Indices[i].z;
		
		glm::vec3 e1 = Vertices[ia].position - Vertices[ib].position;
		glm::vec3 e2 = Vertices[ic].position - Vertices[ib].position;
		glm::vec3 no = cross(e1, e2);

		Vertices[ia].normal += no;
		Vertices[ib].normal += no;
		Vertices[ic].normal += no;
	}

	for (int i = 0; i < vertexCnt + 1; i++) {
		Vertices[i].normal = normalize(Vertices[i].normal);
		cout << Vertices[i].normal.x << " " << Vertices[i].normal.y << " " << Vertices[i].normal.z << endl;
	}
}

GLuint MyModel::Init(char path[])
{
	if (bInitialized) return vao;

	int pIndex = 0;
	int fIndex = 0;

	glm::vec3 max = glm::vec3(-1, -1, -1);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	FILE *file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file!\n");
		return false;
	}

	while (1) {
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;
		if (strcmp(lineHeader, "v") == 0) NumVertices++;
		else if (strcmp(lineHeader, "f") == 0) NumFaces++;	
	}
	vertexCnt = NumVertices;
	NumVertices *= sizeof(float) * sizeof(glm::vec4);
	Vertices = (MyModelVertex*)malloc(sizeof(MyModelVertex)*NumVertices);
	Indices = (glm::vec3*)malloc(sizeof(glm::vec3)*NumFaces);
	glm::vec4 *vertex_positions = (glm::vec4*)malloc(sizeof(glm::vec4)*NumVertices);
	glm::vec4 vertex_colors[1] = { glm::vec4(1.0, 1.0, 1.0, 1.0) };

	file = fopen(path, "r");

	while (1) {
		char lineHeader[128];
	
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex_positions[pIndex] = { vertex.x,vertex.y,vertex.z,1.0 };
			center += vertex;

			if (vertex.x > max.x) max.x = vertex.x;
			if (vertex.y > max.y) max.y = vertex.y;
			if (vertex.z > max.z) max.z = vertex.z;
			//cout << max.x << " " << max.y << " " << max.z << endl;
			//printf("%f %f %f\n", vertex_positions[pIndex].x, vertex_positions[pIndex].y, vertex_positions[pIndex].z);
			pIndex++;
		}
		else if (strcmp(lineHeader, "f") == 0) {
			glm::vec3 face;
			fscanf(file, "%f %f %f\n", &face.x, &face.y, &face.z);
			Indices[fIndex] = { face.x - 1.0, face.y - 1.0, face.z - 1.0 };
			//printf("%f %f %f\n", Indices[fIndex].x, Indices[fIndex].y, Indices[fIndex].z);
			fIndex++;
		}
	}

	center.x = (float)center.x / (float)vertexCnt;
	center.y = (float)center.y / (float)vertexCnt;
	center.z = (float)center.z / (float)vertexCnt;

	scale.x = (1.0 - max.x) * 10 + 1;
	scale.y = (1.0 - max.y) * 10 + 1;
	scale.z = (1.0 - max.z) * 10 + 1;

	if (scale.x > scale.y) {
		if (scale.y > scale.z) {
			Scale = scale.z;
		}
		else
			Scale = scale.y;
	}
	else if (scale.x < scale.y) {
		if (scale.x < scale.z)
			Scale = scale.x;
		else
			Scale = scale.z;
	}

	//cout << center.x << " " << center.y << " " << center.z << endl;
	ShadeModel(vertex_positions, vertex_colors);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyModelVertex)*NumVertices, Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec3)*NumFaces, Indices, GL_STATIC_DRAW);

	bInitialized = true;
	return vao;
}

void MyModel::SetPositionAndColorAttribute(GLuint program)
{
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyModelVertex), BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyModelVertex), BUFFER_OFFSET(sizeof(glm::vec4)));

	//+ normal property setting
	vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(MyModelVertex), BUFFER_OFFSET(2*sizeof(glm::vec4)));
}

void MyModel::Draw(GLuint program)
{
	//cout << "start drawing..." << endl;
	if (!bInitialized) return;			// check whether it is initiazed or not. 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	SetPositionAndColorAttribute(program);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
