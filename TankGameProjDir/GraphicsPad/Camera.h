#pragma once
#include <glm/glm.hpp>
#include <QtGui\QKeyEvent>
#include <Bullet.h>
#include <vector>

class Camera
{
	
	
	glm::vec2 oldMousePosition;
public:
	glm::vec3 UP;
	//std::vector<Bullet*> bulletVec;
	glm::vec3 viewDirection;
	glm::vec3 position;
	float angletank;
	float anglegun;
	glm::vec3 tankDirection;
	glm::vec3 gunDirection;
	Camera();
	void keyUpdate(QKeyEvent* e);
	void mouseUpdate(const glm::vec2& newMousePosition);
	const glm::mat4 getWorldToViewMatrix();
};

