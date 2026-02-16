#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Shader
{
public:
	Shader();
	~Shader();


private:
	GLuint _program = { 0 };

};