#include "../core.h"

class Geometry
{
public:
	Geometry();
	~Geometry();

	static Geometry* create_box(float size);
	static Geometry* create_sphere(float radius);
	static Geometry* create_square(float size);
	static Geometry* create_triangular_pyramid(float size);

	GLuint get_VAO()const { return _VAO; }
	uint32_t get_indices_count()const { return _indices_count; }

private:
	GLuint _VAO = { 0 };
	GLuint _pos_VBO = { 0 };
	GLuint _uv_VBO = { 0 };
	GLuint _EBO = { 0 };

	uint32_t _indices_count = { 0 };

};