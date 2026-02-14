#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)//设置窗口回调
{
    std::cout << "new window size:" << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

void key_call_back(GLFWwindow* window, int key, int scan_code, int action, int mods)//设置按键回调
{
    if (key == GLFW_KEY_W) {}//按下w
    if (key == GLFW_PRESS) {}//按下按键
    if (key == GLFW_RELEASE) {}//抬起按键
    if (key == GLFW_MOD_CONTROL) {}//按下contral同时按下按键
    if (key == GLFW_MOD_SHIFT) {}//按下shift同时按下按键

    std::cout << "key:" << key << " " << scan_code<<" "<<action<< " " << mods << std::endl;
}

int main()
{
	std::cout << "OpenGL" << std::endl;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式

    GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",nullptr,nullptr);//窗体对象
    glfwMakeContextCurrent(window);//设置绘制对象

    //设置监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//监听窗口回调
    glfwSetKeyCallback(window, key_call_back);

    //使用glad加载openGL的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);//设置视口

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        glfwSwapBuffers(window);
    }


    glfwTerminate();

	return 0;
}