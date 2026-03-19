#include <iostream>

#include "wrapper/check_error.h"
#include "application/application.h"
#include "GLframework/shader.h"
#include "GLframework/texture.h"

#include "application/camera/perspective_camera.h"
#include "application/camera/orthographic_camera.h"

#include "application/camera/track_ball_camera_control.h"
#include "application/camera/game_camera_control.h"



GLuint VAO;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);

PerspectiveCamera* camera = nullptr;
//OrthographicCamera* camera = nullptr;

GameCameraControl* camera_control = nullptr;
//TrackBallCameraControl* camera_control = nullptr;

void on_resize(int width, int height)
{
    std::cout << "new window size:" << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

void on_key(int key,int scan_code, int action, int mods)
{
    camera_control->on_key(key, action, mods);
}

void on_mouse(int button, int action, int mods)
{
    double x, y;
    APP->get_cursor_position(&x, &y);
    camera_control->on_mouse(button, action, x, y);
}

void on_motion(double x_pos, double y_pos)
{
    camera_control->on_cursor(x_pos, y_pos);
}
//
void on_scroll(double offset)
{
    camera_control->on_scroll(offset);
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
    //camera = new OrthographicCamera(-5.0f, 5.0f, 5.0f, -5.0f, 5.0f, -5.0f);
    camera = new PerspectiveCamera(60.0f, ((float)APP->get_width() / (float)APP->get_height()), 0.1f, 1000.0f);

    //camera_control = new TrackBallCameraControl();
    camera_control = new GameCameraControl();
    camera_control->set_camera(camera);
}


void render()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //绑定program
    shader->begin();
    shader->set_int("sampler",0);
    shader->set_matrix_4b4("transform", transform);
    shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
    shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());


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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    APP->set_resize_callback(on_resize);
    APP->set_mouse_callback(on_mouse);
    APP->set_cursor_callback(on_motion);
    APP->set_key_callback(on_key);
    APP->set_scroll_callback(on_scroll);

    while (true)
    {
        camera_control->on_update();
        render();
        if (!APP->update())
            break;
    }

    APP->destroy();

	return 0;
}