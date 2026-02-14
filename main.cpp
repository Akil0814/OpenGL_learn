#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"wrapper/check_error.h"
#include"application/application.h"

//void key_call_back(GLFWwindow* window, int key, int scan_code, int action, int mods)//设置按键回调
//{
//    if (key == GLFW_KEY_W) {}//按下w
//    if (key == GLFW_PRESS) {}//按下按键
//    if (key == GLFW_RELEASE) {}//抬起按键
//    if (key == GLFW_MOD_CONTROL) {}//按下contral同时按下按键
//    if (key == GLFW_MOD_SHIFT) {}//按下shift同时按下按键
//
//    std::cout << "key:" << key << " " << scan_code<<" "<<action<< " " << mods << std::endl;
//}
//
void on_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "on_re" << std::endl;

}

int main()
{
	std::cout << "OpenGL" << std::endl;
    if (!APP->init(800, 600))
        return -1;

    APP->set_resize_callback(on_resize);
    //设置监听
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//监听窗口回调
    //glfwSetKeyCallback(window, key_call_back);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (APP->update())
    {
        glClear(GL_COLOR_BUFFER_BIT);

    }

    APP->destroy();

	return 0;
}