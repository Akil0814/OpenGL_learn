#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#define APP Application::instance()

using ResizeCallback = void(*)(int width, int height);
using KeyCallback = void(*)(int key, int scan_code, int action, int mods);
using MouseCallback = void(*)(int button, int action, int mods);
using CursorCallback = void(*)(double x_pos, double y_pos);
using ScrollCallBack = void(*)(double offset);

class Application
{
public:
	static Application* instance();

	bool init(const int& width=800, const int& height=600);

	bool update();

	bool destroy();

	GLFWwindow* get_window() { return _window; }
	uint32_t get_width()const {return _width;}
	uint32_t get_height()const {return _height;}
	void get_cursor_position(double* x, double* y);

	void set_resize_callback(ResizeCallback callback) { _resize_callback = callback; }
	void set_key_callback(KeyCallback callback) { _key_callback = callback; }
	void set_mouse_callback(MouseCallback callback) { _mouse_callback = callback; }
	void set_cursor_callback(CursorCallback callback) { _cursor_callback = callback; }
	void set_scroll_callback(ScrollCallBack callback) { _scroll_callback = callback; }


private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double x_pos, double y_pos);
	static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

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
	KeyCallback _key_callback = { nullptr };
	MouseCallback _mouse_callback = { nullptr };
	CursorCallback _cursor_callback = { nullptr };
	ScrollCallBack _scroll_callback = { nullptr };

};