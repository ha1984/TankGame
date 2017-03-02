#pragma once
//#include <MeGlWindow.h>
#include <glm/glm.hpp>
//#include <QtCore/QObject>

class Particle //: public MeGlWindow
{
public:
	glm::vec3 startPosition;
	long startTime;
	glm::vec3 position;
	glm::vec3 direction;
	float velocity;
	glm::vec3 rotationAxis;
	int time;
	Particle(glm::vec3 pos, glm::vec3 dir, glm::vec3 rot);
	~Particle();
	void move();
};

