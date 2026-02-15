#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"wrapper/check_error.h"
#include"application/application.h"

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

void prepare_interleaved_buffer()
{
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f,  0.0f, 1.0f
    };

    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //描述位置信息
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);
}

int main()
{
	std::cout << "OpenGL" << std::endl;
    if (!APP->init(800, 600))
        return -1;

    prepare_single_buffer();
    prepare_interleaved_buffer();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (APP->update())
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    APP->destroy();

	return 0;
}