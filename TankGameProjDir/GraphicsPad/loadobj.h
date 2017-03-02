#pragma once
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class loadobj
{
public:
	void load_obj(const char* filename, std::vector<glm::vec4> &textures, std::vector<glm::vec4> &vertices, std::vector<glm::vec3> &normals, std::vector<int> &elements);
};