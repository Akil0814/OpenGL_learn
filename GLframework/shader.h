#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include"../wrapper/check_error.h"


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

	void set_vector3(const std::string& name, float x, float y, float z);
	void set_vector3(const std::string& name, const float* values_3);
	void set_vector3(const std::string& name, const glm::vec3 val);


	void set_int(const std::string& name, int value);

	void set_matrix_4b4(const std::string& name,glm::mat4 value);
	void set_matrix_3b3(const std::string& name, glm::mat3 value);


private:
	void check_shader_errors(GLuint target, Type type);

private:
	GLuint _program = { 0 };

};