#include "Assimp_loader.h"
#include "../GLframework/tools/tools.h"
#include "../GLframework/material/phong_material.h"

Object* AssimpLoader::load(const std::string& path)
{
	Object* root_node = new Object();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	//验证读取是否正确
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error: Model Read Failed " << std::endl;
		delete root_node;
		return nullptr;
	}

	process_node(scene->mRootNode,root_node,scene);

	return root_node;
}

void AssimpLoader::process_node(aiNode* ainode, Object* parent, const aiScene* scene)
{
	Object* node = new Object();
	parent->add_child(node);

	glm::mat4 local_matrix = get_mat4f(ainode->mTransformation);
	//位置 旋转 缩放 
	glm::vec3 position, Euler_angle, scale;
	Tools::decompose(local_matrix, position, Euler_angle, scale);
	node->set_position(position);
	node->set_angle_x(Euler_angle.x);
	node->set_angle_y(Euler_angle.y);
	node->set_angle_z(Euler_angle.z);
	node->set_scale(scale);

	//检查有没有mesh
	for (int i = 0; i < ainode->mNumMeshes; ++i)
	{
		int mesh_ID = ainode->mMeshes[i];
		aiMesh* ai_mesh = scene->mMeshes[mesh_ID];
		Mesh* mesh = process_mesh(ai_mesh);
		node->add_child(mesh);
	}

	for (int i = 0; i < ainode->mNumChildren; ++i)
	{
		process_node(ainode->mChildren[i],node,scene);
	}
}

Mesh* AssimpLoader::process_mesh(aiMesh* aimesh)
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<unsigned int> indices;

	positions.reserve(aimesh->mNumVertices * 3);
	normals.reserve(aimesh->mNumVertices * 3);
	uvs.reserve(aimesh->mNumVertices * 2);


	for (int i = 0; i < aimesh->mNumVertices; ++i)
	{
		//顶点数据
		positions.push_back(aimesh->mVertices[i].x);
		positions.push_back(aimesh->mVertices[i].y);
		positions.push_back(aimesh->mVertices[i].z);

		//顶点法线
		normals.push_back(aimesh->mNormals[i].x);
		normals.push_back(aimesh->mNormals[i].y);
		normals.push_back(aimesh->mNormals[i].z);

		//顶点uv
		//关注其第0套uv，一般情况下是贴图uv
		if(aimesh->mTextureCoords[0])
		{
			uvs.push_back(aimesh->mTextureCoords[0][i].x);
			uvs.push_back(aimesh->mTextureCoords[0][i].y);
		}
		else
		{
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
		}

	}


	indices.reserve(aimesh->mNumFaces * 3);
	//解析索引值
	for (int i = 0; i < aimesh->mNumFaces; ++i)
	{
		aiFace face = aimesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto geometry = new Geometry(positions, normals, uvs, indices);
	auto material = new PhongMaterial();
	material->_diffuse = new Texture("assets/textures/Unagi_Body_D.png", 0);
	//material->_diffuse = new Texture("assets/textures/Unagi_Face_D.png", 1);
	//material->_diffuse = new Texture("assets/textures/Unagi_Hair_D.png", 0);


	return new Mesh(geometry,material);
}

glm::mat4 AssimpLoader::get_mat4f(aiMatrix4x4 value)
{
	glm::mat4 to(
		value.a1, value.a2, value.a3, value.a4,
		value.b1, value.b2, value.b3, value.b4,
		value.c1, value.c2, value.c3, value.c4,
		value.d1, value.d2, value.d3, value.d4
		);

	return to;
}

