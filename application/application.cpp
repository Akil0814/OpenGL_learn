#include"application.h"

Application* Application::_instance = nullptr;

Application* Application::instance()
{
	if (_instance == nullptr)
	{
		_instance = new Application();
	}

	return _instance;
}

Application::Application() = default;

Application::~Application() = default;

bool Application::init(const int& width, const int& height)
{
	_width = width;
	_height = height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式

	_window = glfwCreateWindow(_width, _height, "LearnOpenGL", nullptr, nullptr);//窗体对象
	if (_window == NULL)
		return false;

	glfwMakeContextCurrent(_window);//设置绘制对象

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	glfwSetWindowUserPointer(_window, this);

	//键盘响应
	glfwSetKeyCallback(_window, key_callback);

	//鼠标按键响应
	glfwSetMouseButtonCallback(_window, mouse_callback);

	//鼠标运动响应
	glfwSetCursorPosCallback(_window, cursor_callback);

	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(_window))
		return false;

	glfwPollEvents();

	glfwSwapBuffers(_window);

	return true;
}

bool Application::destroy()
{
	glfwTerminate();

	return true;
}

void Application::get_cursor_position(double* x, double* y)
{
	glfwGetCursorPos(_window, x, y);
}


void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->_resize_callback != nullptr)
		self->_resize_callback(width, height);
}

void Application::key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->_key_callback != nullptr)
		self->_key_callback(key,scan_code,action,mods);
}

void Application::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->_mouse_callback != nullptr)
		self->_mouse_callback(button, action, mods);
}

void Application::cursor_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->_cursor_callback != nullptr)
		self->_cursor_callback(x_pos, y_pos);
}
