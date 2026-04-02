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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Renderer* renderer = nullptr;
std::vector<Mesh*> meshes{};


DirectionalLight* dir_light = nullptr;
SpotLight* spot_light = nullptr;
std::vector<PointLight*> point_lights = {};

AmbientLight* amb_light = nullptr;

Camera* camera = nullptr;
CameraControl* camera_control = nullptr;

glm::vec3 clear_color(0.0f);

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
    spot_light->_inner_angle = 10.0f;
    spot_light->_outer_angle = 15.0f;

    dir_light = new DirectionalLight();
    dir_light->_direction= glm::vec3(1.0f);

    auto pointLight1 = new PointLight();
    pointLight1->set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    pointLight1->_color = glm::vec3(1.0f, 0.0f, 0.0f);
    pointLight1->_k2 = 0.0f;
    pointLight1->_k1 = 0.0f;
    pointLight1->_kc = 1.0f;
    point_lights.push_back(pointLight1);

    auto pointLight2 = new PointLight();
    pointLight2->set_position(glm::vec3(0.0f, 8.0f, 0.0f));
    pointLight2->_color = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLight2->_k2 = 0.0f;
    pointLight2->_k1 = 0.0f;
    pointLight2->_kc = 1.0f;
    point_lights.push_back(pointLight2);

    auto pointLight3 = new PointLight();
    pointLight3->set_position(glm::vec3(0.0f, -8.0f, 0.0f));
    pointLight3->_color = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLight3->_k2 = 0.0f;
    pointLight3->_k1 = 0.0f;
    pointLight3->_kc = 1.0f;
    point_lights.push_back(pointLight3);

    auto pointLight4 = new PointLight();
    pointLight4->set_position(glm::vec3(0.0f, 0.0f, -8.0f));
    pointLight3->_color = glm::vec3(1.0f, 1.0f, 0.0f);
    pointLight4->_k2 = 0.0f;
    pointLight4->_k1 = 0.0f;
    pointLight4->_kc = 1.0f;
    point_lights.push_back(pointLight4);

    amb_light = new AmbientLight();
    amb_light->_color = glm::vec3(0.1f);
}

void prepare_camera()
{
    //camera = new OrthographicCamera(-5.0f, 5.0f, 5.0f, -5.0f, 5.0f, -5.0f,{0.0f,0.0f,7.0f});
    camera = new PerspectiveCamera(60.0f, ((float)APP->get_width() / (float)APP->get_height()), 0.1f, 1000.0f, {0.0f,0.0f,10.0f});

    //camera_control = new TrackBallCameraControl();
    camera_control = new GameCameraControl();
    camera_control->set_camera(camera);
}

void initIMGUI()
{
    ImGui::CreateContext();//创建上下文
    ImGui::StyleColorsDark();

    float uiScale = 1.5f;

    ImGui_ImplGlfw_InitForOpenGL(APP->get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void render_imgui()
{
    //1 开启当前imgui渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();


    //2 决定当前GUI上面有哪些控件 从上到下
    ImGui::Begin("begin !");
    ImGui::Text("color");
    ImGui::Button("button",ImVec2{40,20});
    ImGui::ColorEdit3("Clear Color", (float*)& clear_color);
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();


    //3 执行ui渲染
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(APP->get_window(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);//重置视口

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
	std::cout << "OpenGL" << std::endl;
    if (!APP->init(1080, 720))
        return -1;

    prepare();
    prepare_camera();
    initIMGUI();


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
        spot_light->set_position(camera->_position);
        spot_light->_target_direction = (glm::cross(camera->_up, camera->_right));
        renderer->on_render(meshes,camera,nullptr,point_lights,spot_light,amb_light);
        render_imgui();
        glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);

        if (!APP->update())
            break;
    }

    APP->destroy();

	return 0;
}