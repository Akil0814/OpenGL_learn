#pragma once
#include "core.h"

class Object
{
public:
	Object();
	~Object();

	//增量旋转
	void rotate_x(float angle);
	void rotate_y(float angle);
	void rotate_z(float angle);

	void set_position(glm::vec3 pos);
	void set_scale(glm::vec3 scale);

	glm::mat4 get_model_matrix() const;
	glm::vec3 get_position()const;

	void set_parent(Object* obj);
	void add_child(Object* obj);
	std::vector<Object*> get_children();
	Object* get_parent();

protected:
	glm::vec3 _position{ 0.0f };

	float _angle_x = { 0.0f };
	float _angle_y = { 0.0f };
	float _angle_z = { 0.0f };

	glm::vec3 _scale{ 1.0 };

	//父子关系
	std::vector<Object*> _children{};
	Object* _parent = nullptr;
};