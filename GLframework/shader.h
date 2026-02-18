#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"../wrapper/check_error.h"

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
private:
	enum class Type
	{
		COMPILE_VERTEX,
		COMPILE_FRAGMENT,
		LINK_PROGRAM
	};

public:

	Shader(const char* vertex_path, const char* fragment_path);
	~Shader();

	void begin();//开始使用当前Shader

	void end();//结束使用当前shader

	void set_float(const std::string& name, float value);

private:
	void check_shader_errors(GLuint target, Type type);

private:
	GLuint _program = { 0 };

};