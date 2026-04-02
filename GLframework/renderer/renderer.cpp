#include "renderer.h"
#include "../material/phong_material.h"
#include "../material/white_material.h"

Renderer::Renderer()
{
	_phong_shader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	_white_shader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");

}
Renderer::~Renderer()
{}


void Renderer::on_render(const std::vector<Mesh*>& meshes,
	Camera* camera,
	DirectionalLight* dir_light,
	PointLight* point_light,
	SpotLight* spot_light,
	AmbientLight* amb_light)
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

			//高光蒙版的帧更新
			shader->set_int("specular_mask_sampler", 1);
			phong_mat->_specular_mask->bind();


			//mvp变换矩阵
			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
			//获取视点
			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());

			auto normal_matrix = glm::mat3(glm::transpose(glm::inverse(mesh->get_model_matrix())));
			shader->set_matrix_3b3("normalMatrix", normal_matrix);

			//更新光源参数
			//探照灯
			shader->set_vector3("spotLight.position", spot_light->get_position());
			shader->set_vector3("spotLight.color", spot_light->_color);
			shader->set_vector3("spotLight.targetDirection", spot_light->_target_direction);

			shader->set_float("spotLight.specularIntensity", spot_light->_specular_intensity);
			shader->set_float("spotLight.innerLine", glm::cos(glm::radians(spot_light->_inner_angle)));
			shader->set_float("spotLight.outerLine", glm::cos(glm::radians(spot_light->_outer_angle)));

			//平行光
			shader->set_vector3("directionalLight.color", dir_light->_color);
			shader->set_vector3("directionalLight.direction", dir_light->_direction);
			shader->set_float("directionalLight.specularIntensity", dir_light->_specular_intensity);

			//点光源
			shader->set_vector3("pointLight.color", point_light->_color);
			shader->set_vector3("pointLight.position", point_light->get_position());
			shader->set_float("pointLight.specularIntensity", point_light->_specular_intensity);
			shader->set_float("pointLight.k1", point_light->_k1);
			shader->set_float("pointLight.k2", point_light->_k2);
			shader->set_float("pointLight.kc", point_light->_kc);

			//环境光
			shader->set_vector3("ambientColor", amb_light->_color);

			shader->set_float("shiness", phong_mat->_shiness);

			//相机更新
			shader->set_vector3("cameraPosition", camera->_position);
		}
			break;

		case MaterialType::WhiteMaterial:
		{
			//mvp变换矩阵
			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());
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

	case MaterialType::WhiteMaterial:
		result = _white_shader;
		break;

	default:
		break;
	}

	return result;
}


//void Renderer::on_render(const std::vector<Mesh*>& meshes, Camera* camera,
//	PointLight* point_light, AmbientLight* amb_light)
//{
//	//设置当前帧绘制时，OpenGL的必要状态机参数
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	//清理颜色缓存于深度缓存
//	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
//
//	//遍历mush数字进行绘制
//	for (int i = 0; i < meshes.size(); i++)
//	{
//		auto mesh = meshes[i];
//		auto geometry = mesh->_geometry;
//		auto material = mesh->_material;
//
//		//决定使用哪个shader
//		Shader* shader = pick_shader(material->_type);
//
//		//更新shader中的uniform
//		shader->begin();
//
//		switch (material->_type)
//		{
//		case MaterialType::PhongMaterial:
//		{
//			PhongMaterial* phong_mat = (PhongMaterial*)material;
//
//			//diffuse贴图
//			//设置纹理采样器
//			shader->set_int("sampler", 0);
//			phong_mat->_diffuse->bind();
//
//			//高光蒙版的帧更新
//			shader->set_int("specular_mask_sampler", 1);
//			phong_mat->_specular_mask->bind();
//
//
//			//mvp变换矩阵
//			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
//			//获取视点
//			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
//			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());
//
//			auto normal_matrix = glm::mat3(glm::transpose(glm::inverse(mesh->get_model_matrix())));
//			shader->set_matrix_3b3("normalMatrix", normal_matrix);
//
//			//更新光源参数
//			shader->set_vector3("lightColor", dir_light->_color);
//			shader->set_vector3("lightDirection", dir_light->_direction);
//			shader->set_float("specularIntensity", dir_light->_specular_intensity);
//			shader->set_float("shiness", phong_mat->_shiness);
//
//			shader->set_vector3("ambientColor", amb_light->_color);
//
//			//相机更新
//			shader->set_vector3("cameraPosition", camera->_position);
//		}
//		break;
//
//		case MaterialType::WhiteMaterial:
//		{
//			//mvp变换矩阵
//			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
//			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
//			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());
//		}
//		break;
//
//		default:
//			break;
//		}
//
//		//绑定VAO
//		glBindVertexArray(geometry->get_VAO());
//
//		//执行绘制
//		glDrawElements(GL_TRIANGLES, geometry->get_indices_count(), GL_UNSIGNED_INT, (void*)0);
//	}
//}

//void Renderer::on_render(const std::vector<Mesh*>& meshes, Camera* camera,
//	PointLight* point_light, AmbientLight* amb_light)
//{
//	//设置当前帧绘制时，OpenGL的必要状态机参数
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	//清理颜色缓存于深度缓存
//	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
//
//	//遍历mush数字进行绘制
//	for (int i = 0; i < meshes.size(); i++)
//	{
//		auto mesh = meshes[i];
//		auto geometry = mesh->_geometry;
//		auto material = mesh->_material;
//
//		//决定使用哪个shader
//		Shader* shader = pick_shader(material->_type);
//
//		//更新shader中的uniform
//		shader->begin();
//
//		switch (material->_type)
//		{
//		case MaterialType::PhongMaterial:
//		{
//			PhongMaterial* phong_mat = (PhongMaterial*)material;
//
//			//diffuse贴图
//			//设置纹理采样器
//			shader->set_int("sampler", 0);
//			phong_mat->_diffuse->bind();
//
//			//高光蒙版的帧更新
//			shader->set_int("specular_mask_sampler", 1);
//			phong_mat->_specular_mask->bind();
//
//
//			//mvp变换矩阵
//			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
//			//获取视点
//			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
//			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());
//
//			auto normal_matrix = glm::mat3(glm::transpose(glm::inverse(mesh->get_model_matrix())));
//			shader->set_matrix_3b3("normalMatrix", normal_matrix);
//
//			//更新光源参数
//			shader->set_vector3("lightPosition", point_light->get_position());
//			shader->set_vector3("lightColor", point_light->_color);
//			shader->set_float("specularIntensity", point_light->_specular_intensity);
//			shader->set_float("k1", point_light->_k1);
//			shader->set_float("k2", point_light->_k2);
//			shader->set_float("kc", point_light->_kc);
//
//			shader->set_float("shiness", phong_mat->_shiness);
//
//			shader->set_vector3("ambientColor", amb_light->_color);
//
//			//相机更新
//			shader->set_vector3("cameraPosition", camera->_position);
//		}
//		break;
//
//		case MaterialType::WhiteMaterial:
//		{
//			//mvp变换矩阵
//			shader->set_matrix_4b4("modelMatrix", mesh->get_model_matrix());
//			shader->set_matrix_4b4("viewMatrix", camera->get_view_matrix());
//			shader->set_matrix_4b4("projectionMatrix", camera->get_projection_matrix());
//		}
//		break;
//
//		default:
//			break;
//		}
//
//		//绑定VAO
//		glBindVertexArray(geometry->get_VAO());
//
//		//执行绘制
//		glDrawElements(GL_TRIANGLES, geometry->get_indices_count(), GL_UNSIGNED_INT, (void*)0);
//	}
//}
//
