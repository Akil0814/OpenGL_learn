#include "mesh.h"

Mesh::Mesh(Geometry* geometry, Material* material)
{
	_geometry = geometry;
	_material = material;
}

Mesh::~Mesh()
{

}