#include "Collision.h"


Bomb Collision::test(std::vector<Bullet*>& bulletVec, ShapeData cube, mat4 modelToWorldMatrix){

	Bomb bomb;

	std::vector<vec3> worldPositions(6);
	std::vector<vec3> worldN(6);
	std::vector<vec3> world(6);

	for (int j = 0; j < 6; j++){
		worldPositions[j] = vec3(modelToWorldMatrix*vec4(cube.vertices[4*j].position, 1.0f));
		worldN[j] = vec3(modelToWorldMatrix*vec4(cube.vertices[4 * j].normal, 0.0f));
	}

	std::vector<Bullet*>::iterator it;
	//std::vector<Bullet*>::iterator jt;
	for (it = bulletVec.begin(); it != bulletVec.end();){
		//int i = std::distance(bulletVec.begin(), it);
		for (int j = 0; j < 6; j++){
			world[j] = (*it)->position - worldPositions[j];
		}
		if ((dot(world[0], worldN[0]) < 0) && (dot(world[1], worldN[1]) < 0) && (dot(world[2], worldN[2]) < 0)
			&& (dot(world[3], worldN[3]) < 0) && (dot(world[4], worldN[4]) < 0) && (dot(world[5], worldN[5]) < 0))
		{
			bomb.detonate = true;
			bomb.position = (*it)->position;
			//delete (*it);
			it = bulletVec.erase(it);
		}
		else{
			++it;
		}
	}

	return bomb;
}

