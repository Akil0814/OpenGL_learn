#include "renderer.h"
#include "../material/phong_material.h"

Renderer::Renderer()
{
	_phong_shader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
}
Renderer::~Renderer()
{}

void Renderer::on_render(const std::vector<Mesh*>& meshes,Camera* camera,
				DirectionalLight* dir_light,AmbientLight* amb_light)
{
	//设置当前帧绘制时，OpenGL的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//清理颜色缓存于深度缓存
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//遍历mush数字进行绘制
	for (int i = 0; i < meshes.size(); i++)
	{
		auto mesh = meshes[i];
		auto geometry = mesh->_geometry;
		auto material = mesh->_material;

		//决定使用哪个shader
		Shader* shader = pick_shader(material->_type);

		//更新shader中的uniform
		shader->begin();

		switch (material->_type)
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* phong_mat = (PhongMaterial*)material;

			//diffuse贴图
			//设置纹理采样器
			shader->set_int("sampler", 0);
			phong_mat->_diffuse->bind();

			//mvp变换矩阵
			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
			//获取视点
			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());

			auto normal_matrix = glm::mat3(glm::transpose(glm::inverse(mesh->get_model_matrix())));
			shader->set_matrix_3b3("normalMatrix", normal_matrix);

			//更新光源参数
			shader->set_vector3("lightColor", dir_light->_color);
			shader->set_vector3("lightDirection", dir_light->_direction);
			shader->set_float("specularIntensity", dir_light->_specular_intensity);
			shader->set_float("shiness", phong_mat->_shiness);

			shader->set_vector3("ambientColor", amb_light->_color);

			//相机更新
			shader->set_vector3("cameraPosition", camera->_position);
		}
			break;
		default:
			break;
		}

		//绑定VAO
		glBindVertexArray(geometry->get_VAO());

		//执行绘制
		glDrawElements(GL_TRIANGLES, geometry->get_indices_count(), GL_UNSIGNED_INT, (void*)0);
	}
}

Shader* Renderer::pick_shader(MaterialType type)
{
	Shader* result = nullptr;

	switch (type)
	{
	case MaterialType::PhongMaterial:
		result = _phong_shader;
		break;
	default:
		break;
	}

	return result;
}
