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

	vec4 pos = uModel * vPosition;		// 현재 vertex의 위치
	vec4 light_pos = vec4(1, 1, 1, 1);	// 조명의 위치
	vec4 eye_pos = vec4(0, 0, 0, 1);	// 카메라의 위치

	vec4 N = uModel * vec4(vNormal, 0);	// 노멀 벡터
	vec4 L = light_pos - pos;			// 현재 vertex에서 조명으로의 벡터
	vec4 V = eye_pos - pos;				// 현재 vertex에서 카메라로의 벡터

	vec3 NN = normalize(N.xyz);			// normalization, 단위 벡터로
	vec3 NL = normalize(L.xyz);
	vec3 NV = normalize(V.xyz);
	vec3 R = 2 * (dot(NL, NN))*NN - NL;	// 반사 벡터

	float d = dot(NN, NL);				// 노멀벡터와 조명벡터의 벌어진 정도
	if (d < 0) d = 0;
	float s = dot(R, NV);				// 반사벡터와 카메라벡터의 벌어진 정도
	if (s < 0) s = 0;
	//---> dot product는 cosine 값에 비례
	//---> 0도 일때 1로 가장 크고, 90도 일때 0으로 가장 작음(음수일 때 고려 안함)
	//---> 많이 벌어질수록 약한 빛이 카메라로 들어옴

	float glossy = 100;	// specular 강도 조절
	s = pow(s, glossy);	// 0 <= s <= 1 이기 때문에, 거듭제곱할수록 값이 작아짐.

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