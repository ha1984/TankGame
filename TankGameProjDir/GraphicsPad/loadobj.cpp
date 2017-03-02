#pragma once
#include <loadobj.h>

using namespace std;

void loadobj::load_obj(const char* filename, std::vector<glm::vec4> &textures, std::vector<glm::vec4> &vertices, std::vector<glm::vec3> &normals, std::vector<int> &elements)
{
	ifstream in(filename, ios::in);
	if (!in)
	{
		cout << "Cannot open " << filename << endl; exit(1);
	}

	string line;
	while (getline(in,line))
	{
		if (line.substr(0, 2) == "v ")
		{
			stringstream s(line.substr(2));
			glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "vt")
		{
			stringstream s(line.substr(2));
			glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			textures.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			stringstream s(line.substr(2));
			string item;
			while (getline(s, item, ' ')){
				stringstream ss;
				ss.str(item);
				string str;
				while (getline(ss, str, '/')){
					int index = atoi(item.c_str());
					elements.push_back(index);
				}
			}
		}
		else if (line[0] == '#')
		{
			/* ignoring this line */
		}
		else
		{
			/* ignoring this line */
		}
	}

	
	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	//some index error here:

	//for (GLuint i = 0; i < elements.size(); i += 3)
	//{
	//	GLushort ia = elements[i];
	//	GLushort ib = elements[i + 1];
	//	GLushort ic = elements[i + 2];
	//	glm::vec3 normal = glm::normalize(glm::cross(
	//		glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
	//		glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
	//	normals[ia] = normals[ib] = normals[ic] = normal;
	//}
}