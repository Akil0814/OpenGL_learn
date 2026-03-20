#include "geometry.h"
#include <vector>

Geometry::Geometry()
{

}
Geometry::~Geometry()
{
	if(_VAO!=0)
	glDeleteVertexArrays(1, &_VAO);

	if (_pos_VBO != 0)
	glDeleteBuffers(1, &_pos_VBO);

	if (_uv_VBO != 0)
	glDeleteBuffers(1, &_uv_VBO);

	if (_EBO != 0)
	glDeleteBuffers(1, &_EBO);
}

Geometry* Geometry::create_box(float size)
{
	Geometry* geometry = new Geometry();
	geometry->_indices_count = 36;

	float half_size = size / 2.0f;

	float positions[] = {
		// Front
		-half_size, -half_size,  half_size,		 half_size, -half_size,  half_size,		 half_size,  half_size,  half_size,		-half_size,  half_size,  half_size,
		// Back
		 half_size, -half_size, -half_size,		-half_size, -half_size, -half_size,		-half_size,  half_size, -half_size,		 half_size,  half_size, -half_size,
		// Left
		-half_size, -half_size, -half_size,		-half_size, -half_size,  half_size,		-half_size,  half_size,  half_size,		-half_size,  half_size, -half_size,
		// Right
		 half_size, -half_size,  half_size,		 half_size, -half_size, -half_size,		 half_size,  half_size, -half_size,		 half_size,  half_size,  half_size,
		// Top
		-half_size,  half_size,  half_size,		 half_size,  half_size,  half_size,		 half_size,  half_size, -half_size,		-half_size,  half_size, -half_size,
		// Bottom
		-half_size, -half_size, -half_size,		 half_size, -half_size, -half_size,		 half_size, -half_size,  half_size,		-half_size, -half_size,  half_size
	};

	float uvs[] = {
		// Front
		0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f,
		// Back
		0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f,
		// Left
		0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f,
		// Right
		0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f,
		// Top
		0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f,
		// Bottom
		0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f
	};

	unsigned int indices[] = {
		// Front
		0, 1, 2,		2, 3, 0,
		// Back
		4, 5, 6,		6, 7, 4,
		// Left
		8, 9, 10,		10, 11, 8,
		// Right
		12, 13, 14,		14, 15, 12,
		// Top
		16, 17, 18,		18, 19, 16,
		// Bottom
		20, 21, 22,		22, 23, 20
	};

	// 先创建并绑定 VAO
	glGenVertexArrays(1, &geometry->_VAO);
	glBindVertexArray(geometry->_VAO);

	// 创建 position VBO
	glGenBuffers(1, &geometry->_pos_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->_pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 创建 uv VBO
	glGenBuffers(1, &geometry->_uv_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->_uv_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	// 创建 EBO
	glGenBuffers(1, &geometry->_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return geometry;
}

Geometry* Geometry::create_sphere(float radius)
{
	Geometry* geometry = new Geometry();

	//主要变量的声明
	std::vector<GLfloat>positions{};
	std::vector<GLfloat>uvs{};
	std::vector<GLfloat>indices{};

	//声明纬线与经线的数量
	int num_lat_lines = 60;//纬线
	int num_long_lines = 60;//经线

	//通过两层循环(纬线在外，经线在内)->位置, uv
	for (int i = 0; i <= num_lat_lines; i++)
	{
		for (int j = 0; j <= num_long_lines; j++)
		{
			float phi =i * glm::pi<float>() / num_lat_lines;
			float theta = j * 2 * glm::pi<float>() / num_long_lines;

			float y = radius * cos(phi);
			float x = radius * sin(phi) * cos(theta);
			float z = radius * sin(phi) * sin(theta);

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			//float u = (float)j / (float)num_long_lines;
			//float v = (float)i / (float)num_lat_lines;

			//翻转贴图
			float u = 1.0 - (float)j / (float)num_long_lines;
			float v = 1.0 - (float)i / (float)num_lat_lines;

			uvs.push_back(u);
			uvs.push_back(v);
		}
	}


	//通过两层循环 ->顶点索引
	for (int i = 0; i < num_lat_lines; i++)
	{
		for (int j = 0; j < num_long_lines; j++)
		{
			int p1 = i * (num_long_lines + 1) + j;
			int p2 = p1 + num_long_lines + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);

			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);
		}
	}

	//生成VBO与VAO
	// 先创建并绑定 VAO
	glGenVertexArrays(1, &geometry->_VAO);
	glBindVertexArray(geometry->_VAO);

	// 创建 position VBO
	glGenBuffers(1, &geometry->_pos_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->_pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size(), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 创建 uv VBO
	glGenBuffers(1, &geometry->_uv_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->_uv_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	// 创建 EBO
	glGenBuffers(1, &geometry->_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	geometry->_indices_count = indices.size();


	return geometry;
}
