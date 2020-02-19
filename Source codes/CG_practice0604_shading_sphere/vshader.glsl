#version 330

in vec4 vPosition;
in vec4 vColor;

uniform mat4 uModel;
uniform mat4 uOrtho;
uniform mat4 uPerspective;

out vec4 color;

void main()
{
	//gl_Position = uOrtho *uModel*vPosition;
	gl_Position = uPerspective * uModel*vPosition;
	//gl_Position = vPosition;
	color = vColor;
}