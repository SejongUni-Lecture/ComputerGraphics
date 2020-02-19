#version 330

in vec4 vPosition;

out vec4 fColor;

void main()
{
	gl_Position = vPosition;

	fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}