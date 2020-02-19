#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
//in vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uPerspective;

uniform vec4 uLocLgt;
uniform vec4 uLocEye;

out vec3 fN;
out vec3 fL;
out vec3 fV;
out vec4 color;
//out vec2 texCoord;

void main()
{
	gl_Position = uPerspective * uModel*vPosition;

	vec4 pos = uModel * vPosition;
	vec4 N = uModel * vec4(vNormal, 0);
	vec4 L = uLocLgt - pos;
	vec4 V = uLocEye - pos;

	fN = normalize(N.xyz);
	fL = normalize(L.xyz);
	fV = normalize(V.xyz);

	////////////////////////

	//texCoord = vTexCoord;
}