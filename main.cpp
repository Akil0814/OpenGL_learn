#include <iostream>

#include "wrapper/check_error.h"
#include "application/application.h"
#include "GLframework/shader.h"
#include "GLframework/texture.h"

#include "application/camera/perspective_camera.h"
#include "application/camera/orthographic_camera.h"

#include "application/camera/track_ball_camera_control.h"
#include "application/camera/game_camera_control.h"

#include "GLframework/geometry.h"
#include "GLframework/material/phong_material.h"
#include "GLframework/mesh.h"
#include "GLframework/renderer/renderer.h"

Renderer* renderer = nullptr;
std::vector<Mesh*> meshes{};
DirectionalLight* dir_light = nullptr;
AmbientLight* amb_light = nullptr;

Camera* camera = nullptr;
CameraControl* camera_control = nullptr;

Geometry* geometry = nullptr;
Shader* shader = nullptr;
Texture* texture = nullptr;

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

void prepare()
{
    renderer = new Renderer();

    //创建Geometry
    Geometry* geometry = Geometry::create_sphere(6.0f);

    //创建一个material
    auto material = new PhongMaterial();
    material->_shiness = 16.0f;
    material->_diffuse = new Texture("assets/textures/Arcueid_morning.png", 0);
    //生成mesh

    auto mesh = new Mesh(geometry, material);
    meshes.push_back(mesh);

    dir_light = new DirectionalLight();
    amb_light = new AmbientLight();
    amb_light->_color = glm::vec3(0.1f);
}


void prepare_camera()
{
    //camera = new OrthographicCamera(-5.0f, 5.0f, 5.0f, -5.0f, 5.0f, -5.0f,{0.0f,0.0f,7.0f});
    camera = new PerspectiveCamera(60.0f, ((float)APP->get_width() / (float)APP->get_height()), 0.1f, 1000.0f, {0.0f,0.0f,10.0f});

    camera_control = new TrackBallCameraControl();
    //camera_control = new GameCameraControl();
    camera_control->set_camera(camera);
}


int main()
{
	std::cout << "OpenGL" << std::endl;
    if (!APP->init(800, 600))
        return -1;

    prepare();
    prepare_camera();

    glClearColor(0.0f, 0.03f, 0.2f, 1.0f);

    APP->set_resize_callback(on_resize);
    APP->set_mouse_callback(on_mouse);
    APP->set_cursor_callback(on_motion);
    APP->set_key_callback(on_key);
    APP->set_scroll_callback(on_scroll);

    while (true)
    {
        camera_control->on_update();
        renderer->on_render(meshes,camera,dir_light,amb_light);
        if (!APP->update())
            break;
    }

    APP->destroy();

	return 0;
}