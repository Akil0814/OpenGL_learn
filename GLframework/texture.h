#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& path, unsigned int unit);
	~Texture();

	void bind();

	int get_width()const { return _width; }
	int get_height()const { return _height; }


private:
	GLuint _texture = { 0 };
	int _width = { 0 };
	int _height = { 0 };
	int _unit = { 0 };
};