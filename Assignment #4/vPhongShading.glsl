#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
uniform mat4 uModel;
uniform mat4 uPerspective;
uniform mat4 uMat;

uniform vec4 uLocLgt;
uniform vec4 uLocEye;

out vec3 fN;	// flat shading������ vertex���� ������ �����ϰ�
out vec3 fL;	// ������ fragments shader�� �Ѱ� interpolation ������
out vec3 fV;	// phong shading������ ���� ��꿡 �ʿ��� ���͸� ����� �Ŀ�
//out vec4 color;	// ���͸� fragment shader�� �Ѱ� interpolation �ϰ� ������ ���

void main()
{

	vec4 pos = uModel * vPosition;	// ���� vertex�� ��ġ
	vec4 N = uModel * vec4(vNormal, 0);	// ��� ����
	vec4 L = uLocLgt - pos;	// ���� vertex���� ���������� ����
	vec4 V = uLocEye - pos;	// ���� vertex���� ī�޶���� ����

	fN = normalize(N.xyz);	// normalization, ���� ���ͷ�
	fL = normalize(L.xyz);
	fV = normalize(V.xyz);

	gl_Position = uPerspective * uModel*uMat*vPosition;
	//color = vColor;
}