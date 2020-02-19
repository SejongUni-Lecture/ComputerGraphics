#version 330

in vec3 fN;
in vec3 fL;
in vec3 fV;
in vec4 color;
//in vec2 texCoord;

//uniform sampler2D uTex;

uniform vec4 uMAmb;
uniform vec4 uMDif;
uniform vec4 uMSpec;
uniform vec4 uLAmb;
uniform vec4 uLDif;
uniform int uGloss;

out vec4  fColor;

void main()
{
	//vec4 tex = texture(uTex, texCoord);

	vec3 NN = normalize(fN);
	vec3 NL = normalize(fL);
	vec3 NV = normalize(fV);

	vec3 R = 2 * (dot(NL, NN))*NN - NL;

	float d = max(dot(NN, NL), 0);
	float s = max(dot(R, NV), 0);

	s = pow(s, uGloss);

	fColor = uLAmb* uMAmb + uLDif* uMDif* d + uMSpec* uLDif * s;
	//fColor = tex;
}