#pragma once
#include <QtCore/QObject>
#include <glm/glm.hpp>
#include <Particle.h>
#include <vector>

class Explosion :
	public QObject
{
	Q_OBJECT
public:
	std::vector<Particle*> particleVec;
	glm::vec3 position;
	long startTime;
	int elapsedTime;
	Explosion(glm::vec3 pos);
	~Explosion();
public slots:
	void spawn();
};

