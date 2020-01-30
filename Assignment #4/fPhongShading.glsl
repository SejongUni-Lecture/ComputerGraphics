#version 330

in vec3 fN;
in vec3 fL;
in vec3 fV;
in vec4 color;

uniform vec4 uAmb;
uniform vec4 uDif;
uniform vec4 uSpc;
uniform vec4 uMAmb;
uniform vec4 uMDif;
uniform vec4 uMSpec;
uniform vec4 uLAmb;
uniform vec4 uLDif;
uniform int uGloss;

out vec4  fColor;

void main()
{
	vec3 NN = normalize(fN);	// vertex shader���� �Ѱ� �ޱ� ���� normalization�� ������
	vec3 NL = normalize(fL);	// fragment shader�� ���� �������� interpolation�� ����� ���� ���Ͱ� �ƴ� ���°� �� �� �ִ�.
	vec3 NV = normalize(fV);	// �׷��� �ѹ� �� normalization ������ ��ģ��.

	vec3 R = 2 * (dot(NL, NN))*NN - NL;	// �ݻ� ���� ���
	//vec3 R= NV + NL;

	float d = max(dot(NN, NL), 0);	// ��ֺ��Ϳ� �������� ������ ����
	float s = max(dot(R, NV), 0);	// �ݻ纤�Ϳ� ī�޶����� ������ ����

	s = pow(s, uGloss);	// specular ���� ����

	fColor = uAmb + uDif*d + uSpc * s;
	//fColor = vec4(NN,1.0);
	//fColor = (uLAmb* uMAmb) + (uLDif* uMDif* d) + (uMSpec* uLDif * s);
			// ambient + diffuse + specular
	
}