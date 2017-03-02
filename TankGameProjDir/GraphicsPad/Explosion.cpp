#pragma once
#include "Explosion.h"
#include <QtCore/qtimer.h>
#include <glm\gtc\random.hpp>
#include <MeGlWindow.h>
#include <iostream>

#include <omp.h>

extern MeGlWindow * meWindow;

Explosion::Explosion(glm::vec3 pos) : elapsedTime(0)
{
	startTime = meWindow->getTime();
	position = pos;
	//particleVec.resize(10000);
	//spawn a new particle every 20ms:
	//QTimer * timer = new QTimer();
	//QObject::connect(timer, SIGNAL(timeout()), this, SLOT(spawn()));
	//timer->setInterval(1000 / 30);
	//timer->start();

}

void Explosion::spawn(){

	//if (elapsedTime > 10){
	//	delete this;
	//}

	glm::vec3 direction;
	glm::vec3 rotationAxis;

	if (elapsedTime < 10){
		for (int i = 0; i < 1000; i++){
			direction = glm::sphericalRand(2.0f);
			rotationAxis = glm::sphericalRand(1.0f);
			Particle* particle = new Particle(position, direction, rotationAxis);
			particleVec.push_back(particle);
		}
	}

	std::cout << "move particles, elapsedTime = " << elapsedTime << std::endl;

	omp_set_num_threads(4);
	#pragma omp parallel for
	for (int i = 0; i < particleVec.size(); i++){
		particleVec[i]->move();
	}

	//delete and remove explosions without particles:
	//std::vector<Explosion*>::iterator jt;
	//for (jt = meWindow->explosionVec.begin(); jt != meWindow->explosionVec.end();){
	//	//if ((*jt)->particleVec.size()<1){
	//	if (elapsedTime>12){
	//		delete (*jt);
	//		jt = meWindow->explosionVec.erase(jt);
	//	}
	//	else{
	//		++jt;
	//	}
	//}

	elapsedTime += 1;

	//meWindow->repaint();
}

Explosion::~Explosion()
{
}
