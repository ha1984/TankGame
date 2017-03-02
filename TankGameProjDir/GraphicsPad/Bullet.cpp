#include "Bullet.h"
#include <QtCore/qtimer.h>
#include <iostream>
#include <MeGlWindow.h>
#include <Camera.h>

extern MeGlWindow * meWindow;

Bullet::Bullet(glm::vec3 pos, glm::vec3 dir) : time(0) 
{   
	startTime = meWindow->getTime();
	position = pos;
	startPosition = pos;
	direction = dir;

	//QTimer * timer = new QTimer();
	//QObject::connect(timer, SIGNAL(timeout()), this, SLOT(move()));
	//timer->start(20);

}

void Bullet::move(){

	//if ((length(position) > 20)){
	//	delete this;
	//}
	
		std::cout << "bullet lives " << std::endl;
		//position.x += 0.2*direction.x;
		//position.y += 0.2*direction.y;
		//position.z += 0.2*direction.z;
		long t = meWindow->getTime() - startTime;

		position.x = startPosition.x + t*0.005*direction.x;
		position.y = startPosition.y + t*0.005*direction.y;
		position.z = startPosition.z + t*0.005*direction.z;

		meWindow->enemy->move();

		//meWindow->repaint();

		time += 1;

}

Bullet::~Bullet()
{
}