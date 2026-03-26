#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;


out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;



//aPos作为attribute(属性)传入shader 原则上不允许更改
void main()
{
	//将输入位置转化为其次坐标(3维->4维)
	vec4 transfromPosition=vec4(aPos,1.0);

	//做一个中间变量Transfrom Position
	//用于计算四维位置与modelMatrix相乘的中间结构
	transfromPosition=modelMatrix*transfromPosition;

	worldPosition=transfromPosition.xyz;

	gl_Position = projectionMatrix* viewMatrix * vec4(worldPosition,1.0);

	uv=aUV;
	normal=aNormal;
}
