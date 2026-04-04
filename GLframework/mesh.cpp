#include "mesh.h"

Mesh::Mesh(Geometry* geometry, Material* material)
{
	_geometry = geometry;
	_material = material;
	_type = ObjectType::Mesh;
}

Mesh::~Mesh()
{

}