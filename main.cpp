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
#include "GLframework/material/white_material.h"

#include "GLframework/mesh.h"
#include "GLframework/renderer/renderer.h"
#include "GLframework/light/point_light.h"
#include "GLframework/light/spot_light.h"


Renderer* renderer = nullptr;
std::vector<Mesh*> meshes{};


DirectionalLight* dir_light = nullptr;
SpotLight* spot_light = nullptr;
PointLight* point_light = nullptr;

AmbientLight* amb_light = nullptr;

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

void on_scroll(double offset)
{
    camera_control->on_scroll(offset);
}

void prepare()
{
    renderer = new Renderer();

    //创建Geometry
    Geometry* geometry = Geometry::create_box(3.0f);
    //创建一个material
    auto material_1 = new PhongMaterial();
    material_1->_shiness = 16.0f;
    material_1->_diffuse = new Texture("assets/textures/box.png", 0);
    material_1->_specular_mask = new Texture("assets/textures/sp_mask.png", 1);
    auto mesh_1 = new Mesh(geometry, material_1);

    meshes.push_back(mesh_1);

    //创建白色物体
    Geometry* geometry_w = Geometry::create_sphere(0.1f);

    auto material_w = new WhiteMaterial();
    auto mesh_w = new Mesh(geometry_w, material_w);
    mesh_w->set_position(glm::vec3(3.0, 0.0, 0.0));

    meshes.push_back(mesh_w);

    spot_light = new SpotLight();
    spot_light->set_position(mesh_w->get_position());

    spot_light->_target_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    spot_light->_inner_angle = 30.0f;
    spot_light->_outer_angle = 45.0f;

    dir_light = new DirectionalLight();
    dir_light->_direction= glm::vec3(1.0f);

    point_light = new PointLight();
    point_light->set_position(glm::vec3(0.0f, 0.0f, 2.5f));
    point_light->_specular_intensity = 0.5f;
    point_light->_k1 = 0.07f;
    point_light->_k2 = 0.017f;
    point_light->_kc = 1.0f;

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
    if (!APP->init(1080, 720))
        return -1;

    prepare();
    prepare_camera();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    APP->set_resize_callback(on_resize);
    APP->set_mouse_callback(on_mouse);
    APP->set_cursor_callback(on_motion);
    APP->set_key_callback(on_key);
    APP->set_scroll_callback(on_scroll);

    while (true)
    {
        //meshes[1]->rotate_x(0.01f);
        //meshes[1]->rotate_y(0.1f);

        camera_control->on_update();
        renderer->on_render(meshes,camera,dir_light,point_light,spot_light,amb_light);

        if (!APP->update())
            break;
    }

    APP->destroy();

	return 0;
}