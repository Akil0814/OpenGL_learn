#pragma once
#include "../GLframework/core.h"
#include "../GLframework/object.h"
#include "../GLframework/mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class AssimpLoader
{
public:
	static Object* load(const std::string& path);

private:
	static void process_node(aiNode* ainode, Object* parent,const aiScene* scene);

	static Mesh* process_mesh(aiMesh* aimesh);

	static glm::mat4 get_mat4f(aiMatrix4x4 value);


};