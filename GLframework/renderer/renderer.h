#pragma once
#include <vector>

#include "../core.h"
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directional_light.h"
#include "../light/ambient_light.h"
#include "../shader.h"
class Renderer
{
public:
	Renderer();
	~Renderer();

	void on_render(
		const std::vector<Mesh*>& meshes,
		Camera* camera,
		DirectionalLight* dir_light,
		AmbientLight* amb_light
	);
private:
	Shader* pick_shader(MaterialType type);

private:
	Shader* _phong_shader = { nullptr };
	Shader* _white_shader = { nullptr };

};