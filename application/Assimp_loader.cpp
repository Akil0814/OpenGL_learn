#include "Assimp_loader.h"
#include "../GLframework/tools/tools.h"
#include "../GLframework/material/phong_material.h"

Object* AssimpLoader::load(const std::string& path)
{
	//拿出模型所在目录
	std::size_t last_index = path.find_last_of("//");
	std::string root_path = path.substr(0, last_index + 1);
	std::cout << root_path << std::endl;

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

	process_node(scene->mRootNode,root_node,scene,root_path);

	return root_node;
}

void AssimpLoader::process_node(aiNode* ainode, Object* parent, 
	const aiScene* scene, const std::string& root_path)
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
		Mesh* mesh = process_mesh(ai_mesh,scene, root_path);
		node->add_child(mesh);
	}

	for (int i = 0; i < ainode->mNumChildren; ++i)
	{
		process_node(ainode->mChildren[i],node,scene, root_path);
	}
}

Mesh* AssimpLoader::process_mesh(aiMesh* aimesh, const aiScene* scene,
	const std::string& root_path)
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
		if (aimesh->HasNormals()) {
			normals.push_back(aimesh->mNormals[i].x);
			normals.push_back(aimesh->mNormals[i].y);
			normals.push_back(aimesh->mNormals[i].z);
		}
		else {
			normals.push_back(0.0f);
			normals.push_back(0.0f);
			normals.push_back(0.0f);
		}

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

	size_t total_index_count = 0;
	for (unsigned int i = 0; i < aimesh->mNumFaces; ++i)
	{
		total_index_count += aimesh->mFaces[i].mNumIndices;
	}
	indices.reserve(total_index_count);
	std::cout << "number of faces:" << total_index_count << std::endl;

	//解析索引值
	for (unsigned int i = 0; i < aimesh->mNumFaces; ++i)
	{
		const aiFace& face = aimesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto geometry = new Geometry(positions, normals, uvs, indices);
	auto material = new PhongMaterial();

	if (aimesh->mMaterialIndex >= 0)
	{
		Texture* texture = nullptr;
		Texture* specular = nullptr;
		aiMaterial* ai_mat = scene->mMaterials[aimesh->mMaterialIndex];

std::cout << "---------------------------------------------------------------" << std::endl;
std::cout << "diffuse count: "
	<< ai_mat->GetTextureCount(aiTextureType_DIFFUSE) << '\n';
std::cout << "specular count: "
	<< ai_mat->GetTextureCount(aiTextureType_SPECULAR) << '\n';
std::cout << "normal count: "
	<< ai_mat->GetTextureCount(aiTextureType_NORMALS) << '\n';
std::cout << "height count: "
	<< ai_mat->GetTextureCount(aiTextureType_HEIGHT) << '\n';
std::cout << "base color count: "
	<< ai_mat->GetTextureCount(aiTextureType_BASE_COLOR) << '\n';
std::cout << "metalness count: "
	<< ai_mat->GetTextureCount(aiTextureType_METALNESS) << '\n';
std::cout << "roughness count: "
	<< ai_mat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) << '\n';
std::cout << "mesh material index: " << aimesh->mMaterialIndex << '\n';

		//读取DIFFUSE贴图
		std::cout << "DIFFUSE" << std::endl;
		texture = process_texture(ai_mat, aiTextureType_DIFFUSE, scene, root_path);
		if (texture != nullptr)
		{
			std::cout << "have DIFFUSE" << std::endl;
			texture->set_unit(0);
			material->_diffuse = texture;
		}
		else
			material->_diffuse = Texture::create_texture("assets/textures/defaultTexture.jpg", 0);

		//读取SPECULAR贴图
		std::cout << "SPECULAR" << std::endl;
		specular = process_texture(ai_mat, aiTextureType_SPECULAR, scene, root_path);
		if (specular != nullptr)
		{
			std::cout << "Have SPECULAR" << std::endl;
			specular->set_unit(1);
			material->_specular_mask = specular;
		}
		else
			material->_specular_mask = Texture::create_texture("assets/textures/defaultTexture.jpg", 1);
	}
	else
	{
		material->_diffuse = Texture::create_texture("assets/textures/defaultTexture.jpg",0);
	}


	return new Mesh(geometry,material);
}

Texture* AssimpLoader::process_texture(
	const aiMaterial* ai_mat,
	const aiTextureType& type,
	const aiScene* scene,
	const std::string& root_path
)
{
	Texture* texture = nullptr;

	//获取图片读取路径
	aiString aipath;
	ai_mat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);

	if (aipath.Empty())
	{
		std::cout << "path==empty()" << std::endl;
		return nullptr;
	}

	std::cout << "root path: " << root_path << std::endl;
	std::cout << "aipath: " << aipath.C_Str() << std::endl;

	//判读是否是嵌入图片
	const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
	if (aitexture)
	{
		std::cout << "have path included" << std::endl;
		//纹理图片是内嵌的-获取图片信息
		unsigned char* data_in = reinterpret_cast<unsigned char*>(aitexture->pcData);
		uint32_t width_in = aitexture->mWidth;//通常情况下，代表了整张图片大小
		uint32_t height_in = aitexture->mHeight;
		texture = Texture::create_texture_from_memory(aipath.C_Str(), 0, data_in, width_in, height_in);
	}
	else
	{
		std::cout << "don't have path included" << std::endl;
		std::string full_path = root_path + aipath.C_Str();
		std::replace(full_path.begin(), full_path.end(), '\\', '/');
		texture = Texture::create_texture(full_path, 0);
		std::cout << full_path << std::endl;
	}

	return texture;
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

