#pragma once
#include <glm/glm.hpp>
class Enemy
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	void move();
	Enemy();
};

