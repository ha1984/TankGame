#pragma once

#include <glm/glm.hpp>
#include <ShapeData.h>
#include <vector>
#include <Bullet.h>


using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::dot;

struct Bomb {
	vec3 position;
	bool detonate = false;
};

class Collision
{
public:
	static Bomb test(std::vector<Bullet*>& bulletVec, ShapeData cube, mat4 modelToWorldMatrix);
};

