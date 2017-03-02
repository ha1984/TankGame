
#pragma once
#include <MeGlWindow.h>

struct CameraDirections {
	GLenum CubemapFace;
	glm::vec3 direction;
	glm::vec3 UP;
};