#pragma once
#include <QtCore/QObject>
#include <glm/glm.hpp>

class Bullet : public QObject
{ 
	Q_OBJECT
public:
	long startTime;
	glm::vec3 startPosition;
	glm::vec3 position;
	glm::vec3 direction;
	bool dead = false;
	int time;
	Bullet(glm::vec3 pos, glm::vec3 dir);
	~Bullet();
public slots:
	void move();
};