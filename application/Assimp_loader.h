#pragma once
#include "../GLframework/core.h"
#include "../GLframework/object.h"
#include "../GLframework/mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../GLframework/texture.h"


class AssimpLoader
{
public:
	static Object* load(const std::string& path);

private:
	static void process_node(aiNode* ainode, 
		Object* parent,const aiScene* scene,
		const std::string& root_path );

	static Mesh* process_mesh(aiMesh* aimesh, 
		const aiScene* scene, const std::string& root_path);

	static Texture* process_texture(
		const aiMaterial* aimat,
		const aiTextureType& type,
		const aiScene* scene,
		const std::string& root_path
	);

	static glm::mat4 get_mat4f(aiMatrix4x4 value);


};