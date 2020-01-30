#version 330

in layout(location = 0) vec4 vPosition;
in layout(location = 1) vec4 vColor;
out vec4 color;

void main()
{
	float lightness;
	//color=vec4((1.0+vColor[0])/2.0,(1.0+vColor[1])/2.0,(1.0+vColor[2])/2.0, 1.0);
	color=vColor;
	//lightness=(vPosition.x+vPosition.y+vPosition.z)/3.0;
	//color=vec4((1.0+vPosition.xyz)/2.0,1.0);
	//color=vec4((lightness+vColor.xyz)/1.5+0.15, 1.0);

	gl_Position=vPosition;
}