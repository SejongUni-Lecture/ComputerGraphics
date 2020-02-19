#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uPerspective;

out vec4 color;

void main()
{
	gl_Position = uPerspective * uModel*vPosition;

	vec4 pos = uModel * vPosition;		// ���� vertex�� ��ġ
	vec4 light_pos = vec4(1, 1, 1, 1);	// ������ ��ġ
	vec4 eye_pos = vec4(0, 0, 0, 1);	// ī�޶��� ��ġ

	vec4 N = uModel * vec4(vNormal, 0);	// ��� ����
	vec4 L = light_pos - pos;			// ���� vertex���� ���������� ����
	vec4 V = eye_pos - pos;				// ���� vertex���� ī�޶���� ����

	vec3 NN = normalize(N.xyz);			// normalization, ���� ���ͷ�
	vec3 NL = normalize(L.xyz);
	vec3 NV = normalize(V.xyz);
	vec3 R = 2 * (dot(NL, NN))*NN - NL;	// �ݻ� ����

	float d = dot(NN, NL);				// ��ֺ��Ϳ� �������� ������ ����
	if (d < 0) d = 0;
	float s = dot(R, NV);				// �ݻ纤�Ϳ� ī�޶����� ������ ����
	if (s < 0) s = 0;
	//---> dot product�� cosine ���� ���
	//---> 0�� �϶� 1�� ���� ũ��, 90�� �϶� 0���� ���� ����(������ �� ��� ����)
	//---> ���� ���������� ���� ���� ī�޶�� ����

	float glossy = 100;	// specular ���� ����
	s = pow(s, glossy);	// 0 <= s <= 1 �̱� ������, �ŵ������Ҽ��� ���� �۾���.

	// for material
	vec4 ma = vec4(0.7, 0.3, 0.3, 1);	// ambient color and intensity
	vec4 md = vec4(1, 0, 0, 1);			// diffuse color and intensity
	vec4 ms = vec4(1, 1, 0, 1);			// specular color and intensity

	// for light
	vec4 la = vec4(1, 1, 1, 1);		// ambient color and intensity
	vec4 ll = vec4(1, 1, 1, 1);		// diffuse color and intensity

	color = (ma * la) + (md * ll*d) + (ms * ll*s);
			// ambient + diffuse + specular
}