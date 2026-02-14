#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#define APP Application::instance()

using ResizeCallback = void(*)(int width, int height);

class Application
{
public:
	static Application* instance();

	bool init(const int& width=800, const int& height=600);

	bool update();

	bool destroy();


	uint32_t get_width()const
	{
		return _width;
	}

	uint32_t get_height()const
	{
		return _height;
	}

	void set_resize_callback(ResizeCallback callback) { _resize_callback = callback; }

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
	Application();
	Application(Application& copy) = delete;
	~Application();

private:
	static Application* _instance;

	uint32_t _width = { 0 };
	uint32_t _height = { 0 };

	GLFWwindow* _window = { nullptr };

	ResizeCallback _resize_callback = { nullptr };

};