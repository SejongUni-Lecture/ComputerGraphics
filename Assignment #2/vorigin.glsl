#version 150

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

//uniform vec3 theta;
//uniform float scale;
uniform mat4 uMat;
uniform int uTmp;

void main(){
	//Compute the sines and cosines of theta for each of the three axis in one computation
	//vec3 angles = radians(theta);
	//vec3 c = cos(angles);
	//vec3 s = sin(angles);

	/*mat4 rx = mat4(1.0,0.0,0.0,0.0,
				   0.0,c.x,-s.x,0.0,
				   0.0,s.x,c.x,0.0,
				   0.0,0.0,0.0,1.0);
	
	mat4 ry = mat4(c.y,0.0,s.y,0.0,
				   0.0,1.0,0.0,0.0,
				   -s.y,0.0,c.y,0.0,
				   0.0,0.0,0.0,1.0);

	mat4 rz = mat4(c.z,-s.z,0.0,0.0,
				   s.z,c.z,0.0,0.0,
				   0.0,0.0,1.0,0.0,
				   0.0,0.0,0.0,1.0);

	mat4 sc = mat4(scale,0.0,0.0,0.0,
				  0.0,scale,0.0,0.0,
				  0.0,0.0,scale,0.0,
				  0.0,0.0,0.0,1.0);*/

	color=vColor;
	//gl_Position=rx*ry*rz*uMat*vPosition;
	//gl_Position=rx*ry*rz*vPosition;
	//gl_Position=uMat*vPosition;
	gl_Position=vPosition;
}