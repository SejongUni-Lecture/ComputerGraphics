#version 330

in vec4 vPosition2;
in vec4 vColor2;
out vec4 color2;

uniform mat4 uMat2;

void main(){
	color2=vColor2;
	gl_Position=vPosition2 * uMat2;
}