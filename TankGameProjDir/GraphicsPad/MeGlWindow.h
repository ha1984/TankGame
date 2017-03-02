
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <QtOpenGL\qglwidget>
#include <QtGui\QKeyEvent>
#include <QtGui\qmouseevent>
#include <QtCore/QObject>
#include <Explosion.h>
#include <Bullet.h>
#include <ShapeData.h>
#include <Enemy.h>
#include <boost\date_time\posix_time\posix_time.hpp>


typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;


struct Bomb {
	glm::vec3 position;
	bool detonate = false;
};

class MeGlWindow : public QGLWidget
{  
	Q_OBJECT
protected:
	Time t1;
	Bomb collision_test(ShapeData cube, glm::mat4 modelToWorldMatrix);
	void initializeGL();
	//void paintGL();
	void drawing(glm::vec3 tankPosition);
	void sendDataToOpenGL();
	void setupFrameBuffer();
	void sendTextureToOpenGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void renderText();
	void renderExplosion(glm::vec3 position, glm::mat4 worldToProjectionMatrix);
	void renderWater(glm::mat4 worldToProjectionMatrix);
	void renderSky(glm::mat4 worldToProjectionMatrix);
	void renderTerrain(float borderXpos, float borderXneg, float borderZpos, float borderZneg, float translateX, float translateZ, glm::mat4 worldToProjectionMatrix);
	void renderCrystal(float ambient, glm::mat4 worldToProjectionMatrix);
	void renderBlurOnPlane(float eyeDistance, float blurFactor, GLuint& textureId, GLuint& textureIdDepth, GLuint& VAOid, glm::mat4 worldToProjectionMatrix);
	void renderExplosionOnPlane(float eyeDistance, float blurFactor, GLuint& textureId, GLuint& VAOid, glm::mat4 worldToProjectionMatrix);
	void setupWorldToProjections();
public:
	float fov; //fieldofview angle
	long getTime(); //date time in ms
	std::vector<float> readCSV(GLuint i);
	std::vector<int> csvWidths;
	std::vector<Explosion*> explosionVec;
	std::vector<Bullet*> bulletVec;
	std::vector<ShapeData> ShapeDataVec;
	std::vector<GLuint> numIndicesVec;
	std::vector<GLuint> VAOIdVec;
	std::vector<GLuint> IndexDataByteOffsetVec;
	Enemy* enemy = new Enemy();
	Bomb bomb;
	bool selectAnswerc = false;
	bool laserOn = false;
	float speed = 0.1f;
	MeGlWindow();
	~MeGlWindow();
public slots:
    void paintGL();
};

#endif