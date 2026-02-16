#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"wrapper/check_error.h"
#include"application/application.h"

GLuint VAO, program;

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
         0.0f,  0.5f, 0.0f,
    };

    float colors[] =
    {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f
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

    //创建EBO
    GLuint EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //创建VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //绑定VBO EBO 加入属性描述
    //加入位置属性描述
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //加入EBO到当前VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);

}

void prepare_buffer()
{
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,

         0.5f,  0.5f, 0.0f,
         0.8f,  0.8f, 0.0f,
         0.8f,  0.0f, 0.0f
    };

    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //描述位置信息
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void prepare_shader()
{
    //1 完成vs与fs的源代码，并且装入字符串
    const char* vertexShaderSource =
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "color=aColor;\n"
        "}\n\0";

    const char* fragmentShaderSource =
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "in vec3 color;\n"
        "void main()\n"
        "{\n"
        " FragColor = vec4(color, 1.0f);\n"
        "}\n\0";
    //---------------编译-----------------


    //2 创建 Shader 程序（vs、fs）
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    //3 为 shader 程序输入 shader 代码
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    int success = 0;
    char infoLog[1024];//该字符串用来承接错误与否并输出

    glCompileShader(vertex);//编译shader
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);// 检查 vertex 编译结果，并把结果放在success里面
    if (!success)
    {
        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        std::cout << "Error: SHADER COMPILE ERROR --VERTEX" << "\n" << infoLog << std::endl;
    }

    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        std::cout << "Error: SHADER COMPILE ERROR --FRAGMENT" << "\n" << infoLog << std::endl;
    }

    //---------------链接-----------------
    //5 创建一个Program壳子
    program = 0;
    program = glCreateProgram();

    //6 将vs与fs编译好的结果放到program这个壳子里
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    //7 执行program的链接操作，形成最终可执行shader程序
    glLinkProgram(program);
    //检查链接错误
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
    }

    //shader链接完之后要清理,最终想拿到的是program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void render()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //绑定program
    GL_CALL(glUseProgram(program));
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

}

int main()
{
	std::cout << "OpenGL" << std::endl;
    if (!APP->init(800, 600))
        return -1;

    prepare_shader();
    //prepare_buffer();
    prepareVAO();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    APP->set_resize_callback(on_resize);

    while (APP->update())
    {
        render();
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