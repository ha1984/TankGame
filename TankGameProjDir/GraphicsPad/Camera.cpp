#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <Bullet.h>
#include <MeGlWindow.h>

extern MeGlWindow * meWindow;


Camera::Camera() : viewDirection(-1.0f, 0.0f, 0.0f), 
UP(0.0f, -1.0f, 0.0f), 
position(30.0, 5.0, -10.0),
anglegun(0), angletank(0), 
tankDirection(-1.0f, 0.0f, 0.0f),
gunDirection(-1.0f, 0.0f, 0.0f)
{

}

void Camera::keyUpdate(QKeyEvent* event){
	if (event->key() == Qt::Key_A){
		tankDirection = glm::mat3(glm::rotate(+0.05f, glm::vec3(0.0f, 1.0f, 0.0f))) * tankDirection;
		angletank += 0.05f;
	}
	else if (event->key() == Qt::Key_D){
		tankDirection = glm::mat3(glm::rotate(-0.05f, glm::vec3(0.0f, 1.0f, 0.0f))) * tankDirection;
		angletank -= 0.05f;
	}
	else if (event->key() == Qt::Key_W){
		position.x = position.x + meWindow->speed*tankDirection.x;
		position.y = position.y + meWindow->speed*tankDirection.y;
		position.z = position.z + meWindow->speed*tankDirection.z;
	}
	else if (event->key() == Qt::Key_S){
		position.x = position.x - meWindow->speed*tankDirection.x;
		position.y = position.y - meWindow->speed*tankDirection.y;
		position.z = position.z - meWindow->speed*tankDirection.z;
	}
	else if (event->key() == Qt::Key_Space){
		Bullet* bullet = new Bullet(position - glm::vec3(0.0, 4.0, 0.0), gunDirection);
		meWindow->bulletVec.push_back(bullet);
	}
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition){

	glm::vec2 MouseDelta = oldMousePosition - newMousePosition;

	if (glm::length(MouseDelta) > 40.0f){
		oldMousePosition = newMousePosition;
		return;
	}

	MouseDelta.x = (double)MouseDelta.x * 0.03;
	MouseDelta.y = (double)MouseDelta.y * 0.03;

	//rotate in x-direction:
	viewDirection = glm::mat3(glm::rotate(-MouseDelta.x, UP)) * viewDirection;
	gunDirection = glm::mat3(glm::rotate(-MouseDelta.x, UP)) * gunDirection;
	anglegun += -MouseDelta.x;

	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	//rotate in y-direction:
	viewDirection = glm::mat3(glm::rotate(-MouseDelta.y, toRotateAround)) * viewDirection;
	//UP = glm::mat3(glm::rotate(-MouseDelta.y, toRotateAround)) * UP;

	oldMousePosition = newMousePosition;
}

const glm::mat4 Camera::getWorldToViewMatrix(){
	return glm::lookAt(position, position + viewDirection, UP); //2nd argument of lookAt: camera looks at this point! (position + viewDirection = lookat point in world-coordinates)
}