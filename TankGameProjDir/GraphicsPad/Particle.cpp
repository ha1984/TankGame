#include "Particle.h"
//#include <QtCore/qtimer.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <cstdlib>

#include <MeGlWindow.h>

extern MeGlWindow * meWindow;

Particle::Particle(glm::vec3 pos, glm::vec3 dir, glm::vec3 rot) : time(0)
{
	startTime = meWindow->getTime();
	//random velocity in interval [vmin,vmax]:
	float vmin = 0.003f;
	float vmax = 0.004f;
	velocity = vmin + (float)(std::rand()) / (float)(RAND_MAX / (vmax - vmin));
	//velocity = 0.1f;
	
	position = pos;
	startPosition = pos;
	direction = dir;
	rotationAxis = rot;

	//QTimer * timer = new QTimer();
	//QObject::connect(timer, SIGNAL(timeout()), this, SLOT(move()));

	//timer->start(5);

	//if (time > 8){
	//	//timer->stop();
	//	//delete timer;
	//	delete this;
	//}
}

void Particle::move(){

	long t = meWindow->getTime() - startTime;

	position.x = startPosition.x + t*velocity*direction.x;
	position.y = startPosition.y + t*velocity*direction.y; // -0.000002*t*t;
	position.z = startPosition.z + t*velocity*direction.z;
	
	/*position.x += velocity*direction.x;
	position.y += velocity*direction.y;
	position.z += velocity*direction.z;*/

	//meWindow->repaint();

	//rotate particle velocity vector: 
	//direction = glm::vec3(glm::rotate(velocity, rotationAxis) * glm::vec4(direction, 0.0f));
	
	//slow down particle:
	velocity = 0.8f*velocity;

	time += 1;
}

Particle::~Particle()
{
}
