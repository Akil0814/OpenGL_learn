#pragma once
#include <vector>

#include "../core.h"
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directional_light.h"
#include "../light/ambient_light.h"
#include "../light/point_light.h"
#include "../light/spot_light.h"
#include "../shader.h"
#include "../scene.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void on_render(
		Scene* scene,
		Camera* camera,
		DirectionalLight* dir_light,
		const std::vector<PointLight*>& point_lights,
		SpotLight* spot_light,
		AmbientLight* amb_light
	);

	void render_object(
		Object* object,
		Camera* camera,
		DirectionalLight* dir_light,
		const std::vector<PointLight*>& point_lights,
		SpotLight* spot_light,
		AmbientLight* amb_light);

private:
	Shader* pick_shader(MaterialType type);

private:
	Shader* _phong_shader = { nullptr };
	Shader* _white_shader = { nullptr };

};