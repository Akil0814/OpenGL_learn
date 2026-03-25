#include <iostream>

#include "wrapper/check_error.h"
#include "application/application.h"
#include "GLframework/shader.h"
#include "GLframework/texture.h"

#include "application/camera/perspective_camera.h"
#include "application/camera/orthographic_camera.h"

#include "application/camera/track_ball_camera_control.h"
#include "application/camera/game_camera_control.h"

#include "GLframework/geometry/geometry.h"


Geometry* geometry = nullptr;
Geometry* geometry2 = nullptr;

Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);

Camera* camera = nullptr;
CameraControl* camera_control = nullptr;

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
    //geometry2 = Geometry::create_box(6.0f);
    //geometry = Geometry::create_sphere(6.0f);
    geometry = Geometry::create_triangular_pyramid(3.0f);
}

void prepare_shader()
{
    shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

void prepare_texture()
{
    texture = new Texture("assets/textures/Arcueid_morning.png",0);
    //texture = new Texture("assets/textures/moon_t.png", 0);

}

void prepare_camera()
{
    //camera = new OrthographicCamera(-5.0f, 5.0f, 5.0f, -5.0f, 5.0f, -5.0f,{0.0f,0.0f,7.0f});
    camera = new PerspectiveCamera(60.0f, ((float)APP->get_width() / (float)APP->get_height()), 0.1f, 1000.0f);

    camera_control = new TrackBallCameraControl();
    //camera_control = new GameCameraControl();
    camera_control->set_camera(camera);
}

void prepare_state()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void render()
{
    //清理颜色缓存于深度缓存
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));

    //绑定program
    shader->begin();
    shader->set_int("sampler",0);
    shader->set_matrix_4b4("transform", transform);
    shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
    shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());

    //绑定vao
    //GL_CALL(glBindVertexArray(geometry2->get_VAO()));
    //GL_CALL(glDrawElements(GL_TRIANGLES, geometry2->get_indices_count(), GL_UNSIGNED_INT, (void*)0));

    GL_CALL(glBindVertexArray(geometry->get_VAO()));
    GL_CALL(glDrawElements(GL_TRIANGLES, geometry->get_indices_count(), GL_UNSIGNED_INT, (void*)0));

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
    prepare_state();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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