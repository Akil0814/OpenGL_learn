#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

uniform float time;
uniform float speed;
out vec3 color;
out vec2 uv;
out float f_time;

void main()
{
	float dx=0.3;
	float offsetX=sin(time*speed)*dx;
	gl_Position = vec4(aPos.x+offsetX, aPos.y, aPos.z, 1.0);
	color=aColor;
	f_time=time;
	float deltaU=time*0.3;
	uv=vec2(aUV.x+deltaU,aUV.y+deltaU);
}