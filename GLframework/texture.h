#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& path, unsigned int unit);
	Texture(
		unsigned char* data_in,
		uint32_t width_in,
		uint32_t height_in,
		unsigned int unit
	);
	~Texture();

	void bind();
	void set_unit(unsigned int unit) { _unit = unit; }

	int get_width()const { return _width; }
	int get_height()const { return _height; }


private:
	GLuint _texture = { 0 };
	int _width = { 0 };
	int _height = { 0 };
	unsigned int _unit = { 0 };
};