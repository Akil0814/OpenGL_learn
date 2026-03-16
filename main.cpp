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

GLuint VAO;
Shader* shader = nullptr;
Texture* grass_texture = nullptr;
Texture* land_texture = nullptr;
Texture* noise_texture = nullptr;

void on_resize(int width, int height)
{
    std::cout << "new window size:" << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

void prepareVAO()
{
    float position[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
    };

    float colors[] =
    {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,
    };


    float uvs[] =
    {
        0.0f, 0.0f,
        2.0f, 0.0f,
        0.0f, 1.0f,
        2.0f, 1.0f,
    };

    unsigned int indices[] =
    {
        0,1,2,
        2,1,3
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
    grass_texture = new Texture("assets/textures/grass.png",0);
    land_texture = new Texture("assets/textures/land.png", 1);
    noise_texture = new Texture("assets/textures/noise.png", 2);
}

void render()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //绑定program
    shader->begin();
    shader->set_float("time",glfwGetTime());

    shader->set_int("grass_sampler",0);
    shader->set_int("land_sampler", 1);
    shader->set_int("noise_sampler", 2);


    shader->set_float("time", glfwGetTime());
    shader->set_float("speed", 0.5);

    // Keep texture units explicitly bound for this draw call.
    grass_texture->bind();
    land_texture->bind();
    noise_texture->bind();

    //绑定vao
    GL_CALL(glBindVertexArray(VAO));

    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));

    //发出绘制指令
    //三角形：
    //GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
    //GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 6));
    //GL_CALL(glDrawArrays(GL_TRIANGLE_FAN, 0, 6));

    //直线：
    //GL_CALL(glDrawArrays(GL_LINES, 0, 6));
    //GL_CALL(glDrawArrays(GL_LINE_STRIP, 0, 6));\

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


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    APP->set_resize_callback(on_resize);

    while (true)
    {
        render();
        if (!APP->update())
            break;
    }

    APP->destroy();

	return 0;
}


void prepare_single_buffer()
{
    float positions[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    float colors[] =
    {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    //生成vbo
    GLuint posVBO = 0, colorVBO = 0;
    glGenBuffers(1, &posVBO);
    glGenBuffers(1, &colorVBO);


    //绑定当前vbo 到OpenGL状态机的当前vbo插槽上
    //GL_ARRAY_BUFFER 表示当前vbo这个插槽
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向当前vbo传输数据 也是在开辟显存

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //描述位置信息
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);//绑定VBO，下面属性描述才会与当前VBO相关
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

}
