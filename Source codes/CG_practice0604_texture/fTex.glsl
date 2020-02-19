#version 330

in vec4 fColor;

out vec4 displayColor;

void main()
{
	int dist = 30;
	int range = 3;

	if (int(gl_FragCoord.x) % dist <= range) displayColor = vec4(1.0, 1.0, 1.0, 1.0);
	else if (int(gl_FragCoord.y) % dist <= range) displayColor = vec4(1.0, 1.0, 1.0, 1.0);
	else displayColor = vec4(0.0, 0.0, 0.0, 1.0);
}