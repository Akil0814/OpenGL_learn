#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "wrapper/check_error.h"
#include "application/application.h"
#include "GLframework/shader.h"
#include "GLframework/texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

GLuint VAO;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);
glm::mat4 view_matrix(1.0f);
glm::mat4 ortho_matrix(1.0f);
glm::mat4 perspective_matrix(1.0f);


void on_resize(int width, int height)
{
    std::cout << "new window size:" << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

void on_mouse(int button, int action, int mods)
{
    std::cout << "mouse" << button << " " << action << " " << mods << std::endl;
}

void on_motion(double x_pos, double y_pos)
{
    std::cout << "cursor" << x_pos << " " << y_pos << std::endl;
}

void prepareVAO()
{
    //float position[] =
    //{
    //    -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f,  0.5f, 0.0f,//中点三角形
    //    //-0.5f,  0.5f, 0.0f,
    //     //0.5f,  0.5f, 0.0f,
    //};

    float position[] =
    {
        -1.0f,  0.0f, 0.0f,
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };

    float colors[] =
    {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         //0.5f,  0.5f, 0.5f,
    };


    float uvs[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,
        //0.0f, 1.0f,
        //2.0f, 1.0f,
    };

    unsigned int indices[] =
    {
        0,1,2,
        //2,1,3
    };

    //创建VBO
    GLuint posVBO = 0;
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    GLuint colorVBO = 0;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint uvVBO = 0;
    glGenBuffers(1, &uvVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    //创建EBO
    GLuint EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //创建VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //绑定VBO EBO 加入属性描述
    //加入位置属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //加入颜色属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //加入uv属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    //加入EBO到当前VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);

}

void prepare_shader()
{
    shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

void prepare_texture()
{
    texture = new Texture("assets/textures/Arcueid_morning.png",0);
}

void prepare_camera()
{
    //lookAt:生成一个view matrix
    //eye:当前摄像机所在的位置
    //center:当前摄像机看向的那个点
    //up:穹顶向量
    view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
}

void prepare_ortho()
{
    //使用glm的other函数，生成了一个正交投影矩阵：生成一个投影盒子，将内部顶点转化到NDC坐标
    ortho_matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f,  2.0f);
}

void prepare_perspective()
{
    //fovy: y轴方向的视张角 aspect:近平面的横纵百分比 near: 近平面距离 far: 远平面距离 up:穹定向量
    perspective_matrix = glm::perspective(glm::radians(60.0f), ((float)APP->get_width() / (float)APP->get_height()), 0.1f, 1000.0f);
}

void render()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //绑定program
    shader->begin();
    shader->set_int("sampler",0);
    shader->set_matrix_4b4("transform", transform);
    shader->set_matrix_4b4("viewMatrix", view_matrix);
    shader->set_matrix_4b4("projectionMatrix", perspective_matrix);


    //绑定vao
    GL_CALL(glBindVertexArray(VAO));

    GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0));

    //发出绘制指令
    //三角形：
    //GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
    //GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 6));
    //GL_CALL(glDrawArrays(GL_TRIANGLE_FAN, 0, 6));

    //直线：
    //GL_CALL(glDrawArrays(GL_LINES, 0, 6));
    //GL_CALL(glDrawArrays(GL_LINE_STRIP, 0, 6));

    glBindVertexArray(0);
    shader->end();
}

int main()
{
	std::cout << "OpenGL" << std::endl;
    if (!APP->init(800, 600))
        return -1;

    prepare_shader();
    prepareVAO();
    prepare_texture();
    prepare_camera();
    prepare_perspective();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    APP->set_resize_callback(on_resize);
    APP->set_mouse_callback(on_mouse);
    APP->set_cursor_callback(on_motion);

    while (true)
    {
        render();
        if (!APP->update())
            break;
    }

    APP->destroy();

	return 0;
}