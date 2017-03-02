#pragma once

#include <MeGlWindow.h>
#include <Vertex.h>
#include <iostream>
#include <fstream> 
#include <ShapeGenerator.h>
#include <Camera.h>
#include <QtGui\qmouseevent>
#include <SOIL2.h>
//#include <Collision.h>
#include <Explosion.h>
#include <Particle.h>
#include <omp.h>
#include <Qt\qsound.h>
#include <QtCore/qtimer.h>
#include <CameraDirections.h>
#include <textbox.h>
#include <loadCSV.h>

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

GLuint programID;
GLuint programIDtext;
GLuint programIDsky;
GLuint programIDwater;
GLuint programIDhorizBlur;
Camera camera;
GLuint fullTransformUniformLocation;
GLuint theBufferID;

GLuint texture0;
GLuint texture1;
GLuint texture2;
GLuint texture3;
GLuint texture4;
GLuint texture5;
GLuint texture6;
GLuint texture7;
GLuint texture8;
GLuint texture9;
GLuint texture10;
GLuint texture11;
GLuint textureIdReflect;
GLuint textureIdRefract;
GLuint textureIdExplosion;
GLuint textureIdCrystal;
GLuint textureIdCrystalFlip;
GLuint textureIdCrystalDepth;
GLuint textureIdCrystalDepthFlip;
GLuint textureIdExplosionDepth;
GLuint textureIdCube[6];
GLuint textureIdShadow;
GLuint rboIdReflect;
GLuint fboIdReflect;
GLuint rboIdRefract;
GLuint rboIdExplosion;
GLuint rboIdCrystal;
GLuint rboIdCrystalFlip;
GLuint rboIdCrystalDepth;
GLuint rboIdCrystalDepthFlip;
GLuint rboIdExplosionDepth;
GLuint rboIdCube[6];
GLuint fboIdRefract;
GLuint fboIdExplosion;
GLuint fboIdCrystal;
GLuint fboIdCrystalFlip;
GLuint fboIdCrystalDepth;
GLuint fboIdCrystalDepthFlip;
GLuint fboIdExplosionDepth;
GLuint fboIdCube[6];
GLuint fboIdShadow;
GLuint m_depth;
bool shadowMapping;
bool crystalDrawing = false;
bool FBOdrawing; //for water reflection render-to-texture
vec3 lightPosition(6.7, 7.0, -6.97);
float waterHeight = -0.1;
GLsizei shadowWidth = 700; // set this high for a high shadow resoulution
GLsizei shadowHeight = 700; // set this high for a high shadow resoulution
GLuint VERTEX_BYTE_SIZE = sizeof(float) * 8;

float zNear = 0.1;
float zFar = 150.0;

CameraDirections cameraDir[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),

	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	vec3(-1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),

	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, -1.0f),

	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	vec3(0.0f, -1.0f, 0.0f),
	vec3(0.0f, 0.0f, -1.0f),

	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, -1.0f, 0.0f),

	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, -1.0f, 0.0f),
};

vector<float> MeGlWindow::readCSV(GLuint i){

	//cout << csvWidths[i] << endl;
	vector<float> TexCoord(8);
	float height = 0.0625f;
	float width = (float)csvWidths[i]/512.0f;
	TexCoord[0] = (float)(i % 16)*0.0625f;
	TexCoord[1] = 1.0f - (float)floor(i / 16)*0.0625f - height;
	TexCoord[2] = (float)(i % 16)*0.0625f;
	TexCoord[3] = 1.0f - (float)floor(i / 16)*0.0625f;
	TexCoord[4] = (float)(i % 16)*0.0625f + width;
	TexCoord[5] = 1.0f - (float)floor(i / 16)*0.0625f;
	TexCoord[6] = (float)(i % 16)*0.0625f + width;
	TexCoord[7] = 1.0f - (float)floor(i / 16)*0.0625f - height;
	return TexCoord;
}

long MeGlWindow::getTime(){

	Time t2(boost::posix_time::microsec_clock::local_time());

	TimeDuration dt = t2 - t1;

	//print formatted date
	//std::cout << dt << std::endl;

	//number of elapsed miliseconds
	long msec = dt.total_milliseconds();

	//print elapsed seconds (with millisecond precision)
	//std::cout << "actual time= " << msec / 1000.0 << " ms" << std::endl;

	return msec;
};

void MeGlWindow::sendDataToOpenGL(){
	loadCSV* csv = new loadCSV();
	csvWidths.resize(256);
	csvWidths = csv->load("FontData5.txt");
	cout << "sendData" << endl;
	// total index incremented only by makeTank:
	int totalvert = 0;
	int totalnorm = 0;
	int totaltext = 0;
	ShapeData data = ShapeGenerator::makeTank("T90_end-14.obj", totalvert, totalnorm, totaltext); //tank0
	ShapeDataVec.push_back(data);
	data = ShapeGenerator::makeTank("T90_end-13.obj", totalvert, totalnorm, totaltext); //tank1
	ShapeDataVec.push_back(data);
	//ShapeData plane = ShapeGenerator::makePlane(10);
	data = ShapeGenerator::makeObject("../../environmentAll3.obj");
	ShapeDataVec.push_back(data);
	data = ShapeGenerator::makePlane(1);
	ShapeDataVec.push_back(data);
	data = ShapeGenerator::makeTriangle();
	ShapeDataVec.push_back(data);
	data = ShapeGenerator::makeObject("../../sky2.obj");
	ShapeDataVec.push_back(data);
	data = ShapeGenerator::makeObject("../../sphere2.obj"); //crystal
	ShapeDataVec.push_back(data);
	for (GLuint i = 0; i < 256; i++){
		vector<float> texCoord = readCSV(i);
		data = ShapeGenerator::makeQuad((float)csvWidths[i]*2.0f/(float)width(), texCoord);
		ShapeDataVec.push_back(data);
	}
	vector<float> texCoord = readCSV(32);
	data = ShapeGenerator::makeBox(1.0f,texCoord); //textbox
	ShapeDataVec.push_back(data);
	data = ShapeGenerator::makeLine(100.0); //Laser
	ShapeDataVec.push_back(data);
	//ShapeData crystal = ShapeGenerator::makeObject("../../Astronaut/Astronaut.obj");
	//ShapeData cube = ShapeGenerator::makeObject("../../ruin2/ruin_triangles.obj"); //cube

	VAOIdVec.resize(ShapeDataVec.size());
	IndexDataByteOffsetVec.resize(ShapeDataVec.size());
	numIndicesVec.resize(ShapeDataVec.size());

	GLuint ByteOffset = 0;

	GLsizeiptr ShapeDataTotalSize = 0;
	for (GLuint i = 0; i < ShapeDataVec.size(); i++){
		ShapeDataTotalSize += ShapeDataVec[i].vertexBufferSize() + ShapeDataVec[i].indexBufferSize();
	}

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER, ShapeDataTotalSize, 0, GL_DYNAMIC_DRAW);			//all objects
	GLsizeiptr currentOffset = 0;
	for (GLuint i = 0; i < ShapeDataVec.size(); i++){
		glBufferSubData(GL_ARRAY_BUFFER, currentOffset, ShapeDataVec[i].vertexBufferSize(), ShapeDataVec[i].vertices); //tank0
		currentOffset += ShapeDataVec[i].vertexBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, currentOffset, ShapeDataVec[i].indexBufferSize(), ShapeDataVec[i].indices);
		currentOffset += ShapeDataVec[i].indexBufferSize();

		numIndicesVec[i] = ShapeDataVec[i].numindices;

		glGenVertexArrays(1, &VAOIdVec[i]); //tank0
		glBindVertexArray(VAOIdVec[i]);
		glEnableVertexAttribArray(0); // 0 steht für die koordinaten der vertices: "location=0" im shadercode
		glEnableVertexAttribArray(1); //  1 steht für die normalen der vertices: "location=1" im shadercode
		glEnableVertexAttribArray(2); // 2 steht für textur der vertices
		glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + 0)); // letztes argument x: koordinaten starten nach x bytes
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + sizeof(float) * 3)); // letztes argument y: farben starten nach y bytes
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + sizeof(float) * 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

		IndexDataByteOffsetVec[i] = ByteOffset + ShapeDataVec[i].vertexBufferSize();
		//all objects before:
		ByteOffset += ShapeDataVec[i].vertexBufferSize() + ShapeDataVec[i].indexBufferSize();	
	}
	
}

void setupTexture(GLuint& textureId, GLuint& fboId, GLuint& rboId, GLsizei& TEXTURE_WIDTH, GLsizei& TEXTURE_HEIGHT){
	// create a texture object
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a renderbuffer object to store depth info
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
		GL_COLOR_ATTACHMENT0,  // 2. attachment point
		GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
		textureId,             // 4. tex ID
		0);                    // 5. mipmap level: 0(base)

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
		GL_DEPTH_ATTACHMENT, // 2. attachment point
		GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
		rboId);              // 4. rbo ID

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE){
		cout << "fbo not used" << endl;
	}

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupDepth(GLuint& textureId, GLuint& fboId, GLuint& rboId, GLsizei& TEXTURE_WIDTH, GLsizei& TEXTURE_HEIGHT){
	// create a texture object
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a renderbuffer object to store depth info
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	

	// attach the texture to FBO color attachment point
	glFramebufferTexture(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
		GL_DEPTH_ATTACHMENT,  // 2. attachment point        // 3. tex target: GL_TEXTURE_2D
		textureId,             // 4. tex ID
		0);                    // 5. mipmap level: 0(base)

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE){
		cout << "fbo not used" << endl;
	}

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupDepthCube(GLuint& textureId, GLuint& fboId, GLuint& m_depth, GLsizei& TEXTURE_WIDTH, GLsizei& TEXTURE_HEIGHT){
	
	// create a texture object
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (GLenum i = 0; i < 6; i++){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);

	// attach the texture to FBO color attachment point
	//glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
	//	GL_DEPTH_ATTACHMENT,  // 2. attachment point        // 3. tex target: GL_TEXTURE_2D
	//	GL_TEXTURE_2D,
	//	m_depth,             // 4. tex ID
	//	0);                    // 5. mipmap level: 0(base)
      



	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE){
		cout << "fbo not used" << endl;
	}

	
	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void MeGlWindow::setupFrameBuffer(){
	GLsizei TEXTURE_WIDTH = width();
	GLsizei TEXTURE_HEIGHT = height();
	//reflection texture:
	setupTexture(textureIdReflect, fboIdReflect, rboIdReflect, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	//refraction texture:
	setupTexture(textureIdRefract, fboIdRefract, rboIdRefract, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	//explosion only texture:
	setupTexture(textureIdExplosion, fboIdExplosion, rboIdExplosion, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	//crystal only texture:
	setupTexture(textureIdCrystal, fboIdCrystal, rboIdCrystal, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	//crystal only texture:
	setupTexture(textureIdCrystalFlip, fboIdCrystalFlip, rboIdCrystalFlip, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	//crystal only depth:
	setupDepth(textureIdCrystalDepth, fboIdCrystalDepth, rboIdCrystalDepth, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	//crystal only depth:
	setupDepth(textureIdCrystalDepthFlip, fboIdCrystalDepthFlip, rboIdCrystalDepthFlip, TEXTURE_WIDTH, TEXTURE_HEIGHT);


	for (int i = 0; i < 6; i++){
		setupDepth(textureIdCube[i], fboIdCube[i], rboIdCube[i], shadowWidth, shadowHeight);
	}

	setupDepthCube(textureIdShadow, fboIdShadow, m_depth, TEXTURE_WIDTH, TEXTURE_HEIGHT);
}

void MeGlWindow::sendTextureToOpenGL(){
	cout << "sendTexture" << endl;
	//texture0:
	int width, height;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	unsigned char* image = SOIL_load_image("yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("ground.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	//image = SOIL_load_image("tiles2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	image = SOIL_load_image("../../TextureAtlasAll3bright.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture3
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	//image = SOIL_load_image("../../streetlamp/Textures/Metal.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	//image = SOIL_load_image("../../ruin2/Textures/ruin.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	image = SOIL_load_image("../../sky2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture4
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("fire2.jpe", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture5
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("dudvmap2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture6
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("normalmap2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture7
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("white.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture8
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("FontAtlas5.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture8
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("FontAtlas6.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture8
	glGenTextures(1, &texture10);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("FontAtlas7.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture8
	glGenTextures(1, &texture11);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	image = SOIL_load_image("red.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Bomb MeGlWindow::collision_test(ShapeData cube, mat4 modelToWorldMatrix){
	Bomb bomb;

	std::vector<vec3> worldPositions(6);
	std::vector<vec3> worldN(6);
	std::vector<vec3> world(6);

	for (int j = 0; j < 6; j++){
		worldPositions[j] = vec3(modelToWorldMatrix*vec4(cube.vertices[4 * j].position, 1.0f));
		//worldN[j] = vec3(modelToWorldMatrix*vec4(cube.vertices[4 * j].normal, 0.0f)); // works without nonuniform scaling
		worldN[j] = cube.vertices[4 * j].normal; // works without rotations
	}

	std::vector<Bullet*>::iterator it;
	for (it = bulletVec.begin(); it != bulletVec.end();){
		for (int j = 0; j < 6; j++){
			world[j] = (*it)->position - worldPositions[j];
		}
		if ((dot(world[0], worldN[0]) < 0) && (dot(world[1], worldN[1]) < 0) && (dot(world[2], worldN[2]) < 0)
			&& (dot(world[3], worldN[3]) < 0) && (dot(world[4], worldN[4]) < 0) && (dot(world[5], worldN[5]) < 0))
		{
			bomb.detonate = true;
			bomb.position = (*it)->position;
			delete (*it);
			it = bulletVec.erase(it);
		}
		else if (length((*it)->position) > 35){
			delete (*it);
			it = bulletVec.erase(it);
		}
		else{
			++it;
		}
	}

	return bomb;
};

void MeGlWindow::paintGL(){
	
	glViewport(0, 0, shadowWidth, shadowHeight); // rescale shapeangle to fit window
	GLint planeUniformLocation = glGetUniformLocation(programID, "plane");
	vec4 clippingPlane = vec4(0.0, -1.0, 0.0, waterHeight); //switch to refraction: multiply clipping plane with (-1)
	glUniform4fv(planeUniformLocation, 1, &clippingPlane[0]);

	vec3 camPos = camera.position;
	vec3 camView = camera.viewDirection;
	vec3 camUP = camera.UP;

	//shadow mapping for point light:
	shadowMapping = true;
	camera.position = lightPosition;
	
	
	for (GLenum i = 0; i < 6; i++){
		
		camera.viewDirection = cameraDir[i].direction;
		camera.UP = cameraDir[i].UP;
		
		glBindFramebuffer(GL_FRAMEBUFFER, fboIdCube[i]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)

		//invisibly draw a scene to a texture directly:
		drawing(camPos);

		//glGenerateMipmap(GL_TEXTURE_2D);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, textureIdCube[i]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	camera.position = camPos;
	camera.viewDirection = camView;
	camera.UP = camUP;
	shadowMapping = false;

	glViewport(0, 0, width(), height()); // rescale shapeangle to fit window

	mat4 viewToProjectionMatrix = glm::perspective(fov, ((float)width()) / height(), zNear, zFar);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	if (crystalDrawing){
		//crystal texture generating (invisibly):
		glBindFramebuffer(GL_FRAMEBUFFER, fboIdCrystal);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
		//invisibly draw a scene to a texture directly:
		renderCrystal(1.0f, worldToProjectionMatrix);
		// unbind FBO and switch back to showing window-system
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, textureIdCrystal);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//crystal depth texture generating (invisibly):
		glBindFramebuffer(GL_FRAMEBUFFER, fboIdCrystalDepth);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
		//invisibly draw a scene to a texture directly:
		renderCrystal(1.0f, worldToProjectionMatrix);
		// unbind FBO and switch back to showing window-system
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, textureIdCrystalDepth);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//explosion texture generating (invisibly):
	glBindFramebuffer(GL_FRAMEBUFFER, fboIdExplosion);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
	//invisibly draw a scene to a texture directly:
	renderExplosion(bomb.position, worldToProjectionMatrix);
	// unbind FBO and switch back to showing window-system
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, textureIdExplosion);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//refraction texture generating (invisibly):
	glBindFramebuffer(GL_FRAMEBUFFER, fboIdRefract);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
	//invisibly draw a scene to a texture directly:
	FBOdrawing = true;
	//remove everything over clipping plane:
	planeUniformLocation = glGetUniformLocation(programID, "plane");
	clippingPlane = vec4(0.0, 1.0, 0.0, -waterHeight); //switch to reflection: multiply clipping plane with (-1)
	glUniform4fv(planeUniformLocation, 1, &clippingPlane[0]);
	drawing(camPos);
	// unbind FBO and switch back to showing window-system
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, textureIdRefract);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//reflection texture generating (invisibly): 
	glBindFramebuffer(GL_FRAMEBUFFER, fboIdReflect);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
	//invisibly draw a scene to a texture directly:
	FBOdrawing = true;
	float distance = 2 * (camera.position.y - waterHeight);
	camera.position.y -= distance;
	camera.viewDirection.y = -camera.viewDirection.y;
	//remove everything under clipping plane:
	planeUniformLocation = glGetUniformLocation(programID, "plane");
	clippingPlane = vec4(0.0, -1.0, 0.0, waterHeight); //switch to refraction: multiply clipping plane with (-1)
	glUniform4fv(planeUniformLocation, 1, &clippingPlane[0]);
	drawing(camPos);
	// unbind FBO and switch back to showing window-system
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, textureIdReflect);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (crystalDrawing){
		viewToProjectionMatrix = glm::perspective(fov, ((float)width()) / height(), zNear, zFar);
		worldToViewMatrix = camera.getWorldToViewMatrix();
		worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
		//crystal texture generating (invisibly):
		glBindFramebuffer(GL_FRAMEBUFFER, fboIdCrystalFlip);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
		//invisibly draw a scene to a texture directly:
		renderCrystal(1.0f, worldToProjectionMatrix);
		// unbind FBO and switch back to showing window-system
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, textureIdCrystalFlip);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//crystal depth texture generating (invisibly):
		glBindFramebuffer(GL_FRAMEBUFFER, fboIdCrystalDepthFlip);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
		//invisibly draw a scene to a texture directly:
		renderCrystal(1.0f, worldToProjectionMatrix);
		// unbind FBO and switch back to showing window-system
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, textureIdCrystalDepthFlip);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FBOdrawing = false;

	//draw full scene visibly:
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera.position.y += distance;
	camera.viewDirection.y = -camera.viewDirection.y;
	//remove everything under clipping plane:
	planeUniformLocation = glGetUniformLocation(programID, "plane");
	clippingPlane = vec4(0.0, -1.0, 0.0, waterHeight); //switch to refraction: multiply clipping plane with (-1)
	glUniform4fv(planeUniformLocation, 1, &clippingPlane[0]);
	drawing(camPos);
};

void MeGlWindow::setupWorldToProjections(){

	mat4 viewToProjectionMatrix = glm::perspective(fov, ((float)width()) / height(), zNear, zFar);
	mat4 worldToProjectionMatrixPosX = viewToProjectionMatrix*glm::lookAt(lightPosition, lightPosition + cameraDir[0].direction, cameraDir[0].UP);
	mat4 worldToProjectionMatrixNegX = viewToProjectionMatrix*glm::lookAt(lightPosition, lightPosition + cameraDir[1].direction, cameraDir[1].UP);
	mat4 worldToProjectionMatrixPosY = viewToProjectionMatrix*glm::lookAt(lightPosition, lightPosition + cameraDir[2].direction, cameraDir[2].UP);
	mat4 worldToProjectionMatrixNegY = viewToProjectionMatrix*glm::lookAt(lightPosition, lightPosition + cameraDir[3].direction, cameraDir[3].UP);
	mat4 worldToProjectionMatrixPosZ = viewToProjectionMatrix*glm::lookAt(lightPosition, lightPosition + cameraDir[4].direction, cameraDir[4].UP);
	mat4 worldToProjectionMatrixNegZ = viewToProjectionMatrix*glm::lookAt(lightPosition, lightPosition + cameraDir[5].direction, cameraDir[5].UP);

	GLint worldToProjectionUniformLocationPosX = glGetUniformLocation(programID, "worldToProjectionPosX");
	GLint worldToProjectionUniformLocationNegX = glGetUniformLocation(programID, "worldToProjectionNegX");
	GLint worldToProjectionUniformLocationPosY = glGetUniformLocation(programID, "worldToProjectionPosY");
	GLint worldToProjectionUniformLocationNegY = glGetUniformLocation(programID, "worldToProjectionNegY");
	GLint worldToProjectionUniformLocationPosZ = glGetUniformLocation(programID, "worldToProjectionPosZ");
	GLint worldToProjectionUniformLocationNegZ = glGetUniformLocation(programID, "worldToProjectionNegZ");

	glUniformMatrix4fv(worldToProjectionUniformLocationPosX, 1, GL_FALSE, &worldToProjectionMatrixPosX[0][0]);
	glUniformMatrix4fv(worldToProjectionUniformLocationNegX, 1, GL_FALSE, &worldToProjectionMatrixNegX[0][0]);
	glUniformMatrix4fv(worldToProjectionUniformLocationPosY, 1, GL_FALSE, &worldToProjectionMatrixPosY[0][0]);
	glUniformMatrix4fv(worldToProjectionUniformLocationNegY, 1, GL_FALSE, &worldToProjectionMatrixNegY[0][0]);
	glUniformMatrix4fv(worldToProjectionUniformLocationPosZ, 1, GL_FALSE, &worldToProjectionMatrixPosZ[0][0]);
	glUniformMatrix4fv(worldToProjectionUniformLocationNegZ, 1, GL_FALSE, &worldToProjectionMatrixNegZ[0][0]);

}

void setupShadowTexures(){
	//shadowMap:
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureIdCube[0]);
	glUniform1i(glGetUniformLocation(programID, "shadowPosX"), 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, textureIdCube[1]);
	glUniform1i(glGetUniformLocation(programID, "shadowNegX"), 2);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, textureIdCube[2]);
	glUniform1i(glGetUniformLocation(programID, "shadowPosY"), 3);

	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, textureIdCube[3]);
	glUniform1i(glGetUniformLocation(programID, "shadowNegY"), 4);

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, textureIdCube[4]);
	glUniform1i(glGetUniformLocation(programID, "shadowPosZ"), 5);

	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, textureIdCube[5]);
	glUniform1i(glGetUniformLocation(programID, "shadowNegZ"), 6);
}

void MeGlWindow::renderText(){
	textbox* text = new textbox("Who are you?");

	glUseProgram(programIDtext);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(programIDtext, "ourTexture1"), 0);
	GLint cursorxUniformLocation = glGetUniformLocation(programIDtext, "cursorX");
	GLint cursoryUniformLocation = glGetUniformLocation(programIDtext, "cursorY");
	int index;
	float cursorx;
	float cursory = 0.0f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	for (int i = 0; i < text->len; i++){
		index = 7 + text->shapeIdx[i];
		cursorx = text->cursor[i];
		glUniform1fv(cursorxUniformLocation, 1, &cursorx);
		glBindVertexArray(VAOIdVec[index]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));
	}

	if (!selectAnswerc){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture9);
		glUniform1i(glGetUniformLocation(programIDtext, "ourTexture1"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glUniform1i(glGetUniformLocation(programIDtext, "ourTexture1"), 0);
	}

	textbox* textb = new textbox("1) I am a Tank!");
	cursory = 0.2f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	for (int i = 0; i < textb->len; i++){
		index = 7 + textb->shapeIdx[i];
		cursorx = textb->cursor[i];
		glUniform1fv(cursorxUniformLocation, 1, &cursorx);
		glBindVertexArray(VAOIdVec[index]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));
	}

	if (!selectAnswerc){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glUniform1i(glGetUniformLocation(programIDtext, "ourTexture1"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture9);
		glUniform1i(glGetUniformLocation(programIDtext, "ourTexture1"), 0);
	}

	textbox* textc = new textbox("2) I am a russian Tank!");
	cursory = 0.28f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	for (int i = 0; i < textc->len; i++){
		index = 7 + textc->shapeIdx[i];
		cursorx = textc->cursor[i];
		glUniform1fv(cursorxUniformLocation, 1, &cursorx);
		glBindVertexArray(VAOIdVec[index]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));
	}

	cursory = 0.0f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	index = 7 + 256;
	cursorx = 0.0f;
	glUniform1fv(cursorxUniformLocation, 1, &cursorx);
	glBindVertexArray(VAOIdVec[index]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));

	glUseProgram(programID);
}

void MeGlWindow::drawing(glm::vec3 tankPosition){
	//glDepthMask(GL_TRUE);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP);


	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(fov, ((float)width()) / height(), zNear, zFar);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	setupWorldToProjections();

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.6f, 0.6f, 0.6f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	GLint alphaUniformLocation = glGetUniformLocation(programID, "alpha");
	float alpha = 1.0f;
	glUniform1fv(alphaUniformLocation, 1, &alpha);

	GLint specularityUniformLocation = glGetUniformLocation(programID, "specularity");
	vec3 specularity(10.0f, 10.0f, 10.0f);
	glUniform3fv(specularityUniformLocation, 1, &specularity[0]);

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPosition");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePosition");
	vec3 eyePosition = camera.position;
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	GLint texShiftUniformLocation = glGetUniformLocation(programID, "texShift");
	glm::vec2 textureShift(0.0f, 0.0f);
	glUniform2fv(texShiftUniformLocation, 1, &textureShift[0]);


	GLint fogColorUniformLocation = glGetUniformLocation(programID, "fogColor");
	vec3 fogColor(0.01f, 0.01f, 0.01f);
	glUniform3fv(fogColorUniformLocation, 1, &fogColor[0]);

	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programID,
		"modelToWorldTransformMatrix");
	GLint fullTransformUniformLocation = glGetUniformLocation(programID,
		"fullTransformMatrix");
	GLint worlToViewUniformLocation = glGetUniformLocation(programID,
		"worldToViewMatrix");

	glUniformMatrix4fv(worlToViewUniformLocation, 1, GL_FALSE, &worldToViewMatrix[0][0]);

	setupShadowTexures();

	//text:
	if (!shadowMapping && !FBOdrawing){
		if (length(camera.position - enemy->position) < 10){
			renderText();
		}
	}
	

	//spherical iglo:
	specularity = vec3(0.0f, 0.0f, 0.0f);
	glUniform3fv(specularityUniformLocation, 1, &specularity[0]);
	renderCrystal(0.6f, worldToProjectionMatrix);

	//Terrain (surrounds lake):
	renderTerrain(0.0f, -6.0f, 5.0f, -5.0f, 0.0f, 0.0f, worldToProjectionMatrix);
	renderTerrain(7.0f, 2.0f, 5.0f, -5.0f, -12.5f, 0.0f, worldToProjectionMatrix);
	renderTerrain(2.0f, 0.0f, 5.0f, 1.0f, 0.0f, -16.5f, worldToProjectionMatrix);
	renderTerrain(2.0f, 0.0f, -1.0f, -5.0f, 0.0f, -9.0f, worldToProjectionMatrix);

	specularity = vec3(10.0f, 10.0f, 10.0f);
	glUniform3fv(specularityUniformLocation, 1, &specularity[0]);

	//tank0:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[0]);
	mat4 tank0ModelToWorldMatrix = glm::translate(tankPosition - glm::vec3(0.0, 5.0, 0.0))* glm::rotate(-1.570796f, vec3(0.0f, 1.0f, 0.0f)) *glm::rotate(camera.anglegun, vec3(0.0f, -1.0f, 0.0f))* glm::translate(vec3(0.0f, 0.0f, 15.0f)) * glm::rotate(-1.570796f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * tank0ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &tank0ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[0], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[0]));

	//tank1:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[1]);
	mat4 tank1ModelToWorldMatrix = glm::translate(tankPosition - glm::vec3(0.0, 5.0, 0.0)) *glm::rotate(-1.570796f, vec3(0.0f, 1.0f, 0.0f)) *glm::rotate(camera.angletank, vec3(0.0f, 1.0f, 0.0f))* glm::translate(vec3(0.0f, 0.0f, 15.0f)) * glm::rotate(-1.570796f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * tank1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &tank1ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[1], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[1]));

	//enemy tank0:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);
	
	glBindVertexArray(VAOIdVec[0]);
	tank0ModelToWorldMatrix = glm::translate(enemy->position)*glm::rotate(-1.570796f, vec3(0.0f, 1.0f, 0.0f))* glm::translate(vec3(0.0f, 0.0f, 15.0f)) * glm::rotate(-1.570796f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * tank0ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &tank0ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[0], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[0]));

	//enemy tank1:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);


	glBindVertexArray(VAOIdVec[1]);
	tank1ModelToWorldMatrix = glm::translate(enemy->position)*glm::rotate(-1.570796f, vec3(0.0f, 1.0f, 0.0f))* glm::translate(vec3(0.0f, 0.0f, 15.0f)) * glm::rotate(-1.570796f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * tank1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &tank1ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[1], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[1]));

	//objekt:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[2]);
	mat4 objektModelToWorldMatrix = mat4();
	fullTransformMatrix = worldToProjectionMatrix * objektModelToWorldMatrix;
	//ambientLight = vec3(0.5f, 0.5f, 0.5f);
	//glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &objektModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[2], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[2]));

	//Laser:
	if (laserOn && !shadowMapping){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture11);
		glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

		glBindVertexArray(VAOIdVec[7 + 257]);
		mat4 laserModelToWorldMatrix = glm::translate(tankPosition - glm::vec3(0.0, 4.0, 0.0))* glm::rotate(-1.570796f, vec3(0.0f, 1.0f, 0.0f)) *glm::rotate(camera.anglegun, vec3(0.0f, -1.0f, 0.0f)) *glm::rotate(1.570796f, vec3(1.0f, 0.0f, 0.0f));
		fullTransformMatrix = worldToProjectionMatrix * laserModelToWorldMatrix;
		//ambientLight = vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
		glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &laserModelToWorldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[7 + 257], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[7 + 257]));
	}
	

	//Crystal:
	//renderCrystal(0.3f, worldToProjectionMatrix);

	//mirror with water shaders:
	if (!FBOdrawing && !shadowMapping){
		renderWater(worldToProjectionMatrix);
	}

	textureShift = glm::vec2(0.0f, 0.0f);
	glUniform2fv(texShiftUniformLocation, 1, &textureShift[0]);

	//bullet:
	mat4 bulletModelToWorldMatrix;
	alpha = 1.0f;
	glUniform1fv(alphaUniformLocation, 1, &alpha);
	//std::cout << "bulletVec.size = " << bulletVec.size() << std::endl;
	for (int i = 0; i < bulletVec.size(); i++){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

		glBindVertexArray(VAOIdVec[3]);
		bulletVec[i]->move();
		bulletModelToWorldMatrix = glm::translate(bulletVec[i]->position)*glm::scale(vec3(0.1f, 0.1f, 0.1f));
		fullTransformMatrix = worldToProjectionMatrix * bulletModelToWorldMatrix;
		ambientLight = vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
		glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &bulletModelToWorldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[3]));
	}

	//sky:
	renderSky(worldToProjectionMatrix);

	//collision detection with cube bounding box for enemy:
	glm::mat4 cubeModelToWorldMatrix = glm::translate(enemy->position - vec3(25.0f, 0.0f, 0.0f))*glm::translate(vec3(24.8f, 0.9f, 0.25f)) * glm::scale(mat4(), vec3(3.8f, 1.4f, 1.75f));
	ShapeData cube = ShapeGenerator::makeCube();
	//Bomb bomb = Collision::test(bulletVec, cube, glm::translate(vec3(0.0f, 1.0f, 6.0f)));
	bomb = collision_test(cube, cubeModelToWorldMatrix);

	if (bomb.detonate){
		QSound sound = QSound::QSound("Bomb.wav");
		sound.play();
		std::cout << "explosion" << std::endl;
		Explosion* explosion = new Explosion(bomb.position);
		explosionVec.push_back(explosion);
	}

	if (!shadowMapping){

		if (crystalDrawing){
			//render bloom for crystal on plane perpendicular to view direction;
			if (!FBOdrawing){
				renderBlurOnPlane(1.1f, 0.3f, textureIdCrystal, textureIdCrystalDepth, VAOIdVec[3], worldToProjectionMatrix);
			}
			else {
				renderBlurOnPlane(1.1f, 0.3f, textureIdCrystalFlip, textureIdCrystalDepthFlip, VAOIdVec[3], worldToProjectionMatrix);
			}
		}

		//render Explosion on plane perpendicular to view direction:
		renderExplosionOnPlane(1.0f, 0.5f, textureIdExplosion, VAOIdVec[3], worldToProjectionMatrix);
	}
}

void MeGlWindow::renderSky(mat4 worldToProjectionMatrix){
	glUseProgram(programIDsky);

	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programIDsky,
		"modelToWorldTransformMatrix");
	GLint fullTransformUniformLocation = glGetUniformLocation(programIDsky,
		"fullTransformMatrix");

	GLint ambientLightUniformLocation = glGetUniformLocation(programIDsky, "ambientLight");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(programIDsky, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[5]);
	mat4 skyModelToWorldMatrix = glm::translate(vec3(camera.position.x, waterHeight + 24.0, camera.position.z))*glm::scale(vec3(50.0f, 50.0f, 50.0f));
	mat4 fullTransformMatrix = worldToProjectionMatrix * skyModelToWorldMatrix;
	vec3 ambientLight = vec3(0.15f, 0.15f, 0.15f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &skyModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[5], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[5]));

	glUseProgram(programID);
}

void MeGlWindow::renderBlurOnPlane(float eyeDistance, float blurFactor, GLuint& textureId, GLuint& textureIdDepth, GLuint& VAOid, mat4 worldToProjectionMatrix){

	vec3 camPosition = camera.position;
	vec3 camDirection = camera.viewDirection;

	glUseProgram(programIDhorizBlur);

	GLint widthUniformLocation = glGetUniformLocation(programIDhorizBlur, "textureWidth");
	float textureWidth = blurFactor*width();
	glUniform1fv(widthUniformLocation, 1, &textureWidth);
	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programIDhorizBlur,
		"modelToWorldTransformMatrix");
	fullTransformUniformLocation = glGetUniformLocation(programIDhorizBlur,
		"fullTransformMatrix");
	//render explosion on this plane:

	vec3 rotAxis = glm::normalize(glm::cross(-camDirection, vec3(0.0, 1.0, 0.0)));
	float rotAngle = glm::acos(dot(-camDirection, vec3(0.0, 1.0, 0.0)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(glGetUniformLocation(programIDhorizBlur, "ourTexture1"), 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureIdDepth);
	glUniform1i(glGetUniformLocation(programIDhorizBlur, "ourTexture2"), 1);

	glBindVertexArray(VAOid);
	GLint depthUniformLocation = glGetUniformLocation(programIDhorizBlur, "depth");
	float depth = 1.0;
	glUniform1fv(depthUniformLocation, 1, &depth);
	mat4 planeModelToWorldMatrix = glm::translate(camPosition + eyeDistance*camDirection)*glm::rotate(-rotAngle, rotAxis)*glm::translate(vec3(-20.0f, 0.0f, -20.0f))*glm::scale(mat4(), vec3(40.0f, 0.0f, 40.0f));
	mat4 fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[3]));

	glUseProgram(programID);
}

void MeGlWindow::renderExplosionOnPlane(float eyeDistance, float blurFactor, GLuint& textureId, GLuint& VAOid, mat4 worldToProjectionMatrix){

	vec3 camPosition = camera.position;
	vec3 camDirection = camera.viewDirection;

	glUseProgram(programIDhorizBlur);

	GLint widthUniformLocation = glGetUniformLocation(programIDhorizBlur, "textureWidth");
	float textureWidth = blurFactor*width();
	glUniform1fv(widthUniformLocation, 1, &textureWidth);
	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programIDhorizBlur,
		"modelToWorldTransformMatrix");
	fullTransformUniformLocation = glGetUniformLocation(programIDhorizBlur,
		"fullTransformMatrix");
	//render explosion on this plane:

	vec3 rotAxis = glm::normalize(glm::cross(-camDirection, vec3(0.0, 1.0, 0.0)));
	float rotAngle = glm::acos(dot(-camDirection, vec3(0.0, 1.0, 0.0)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(glGetUniformLocation(programIDhorizBlur, "ourTexture1"), 0);

	glBindVertexArray(VAOid);
	GLint depthUniformLocation = glGetUniformLocation(programIDhorizBlur, "depth");
	float depth = 0.0;
	glUniform1fv(depthUniformLocation, 1, &depth);
	mat4 planeModelToWorldMatrix = glm::translate(camPosition + eyeDistance*camDirection)*glm::rotate(-rotAngle, rotAxis)*glm::translate(vec3(-20.0f, 0.0f, -20.0f))*glm::scale(mat4(), vec3(40.0f, 0.0f, 40.0f));
	mat4 fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[3]));

	glUseProgram(programID);

}

void MeGlWindow::renderExplosion(glm::vec3 position, mat4 worldToProjectionMatrix){
	
	GLint texShiftUniformLocation = glGetUniformLocation(programID, "texShift");
	GLint alphaUniformLocation = glGetUniformLocation(programID, "alpha");
	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programID,
		"modelToWorldTransformMatrix");

	//explosion particles = triangles:
	mat4 triModelToWorldMatrix;
	vec3 particleAxis = glm::normalize(camera.position - position);
	float particlePhi;
	std::vector<Particle*>::iterator it;
	//tri:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[4]);
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	glm::vec3 ambientLight = vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	std::vector<Explosion*>::iterator jt;
	for (jt = explosionVec.begin(); jt != explosionVec.end();){
		if ((*jt)->elapsedTime>18){
			cout << "explosion duration time= " << getTime() - (*jt)->startTime << " ms" << endl;
			delete (*jt);
			jt = explosionVec.erase(jt);
		}
		else{
			//spawn and move particles:
			(*jt)->spawn();
			//draw fire particles (lifetime=8):
			omp_set_num_threads(4);
            #pragma omp parallel for
			for (int j = 0; j < (*jt)->particleVec.size(); j++){
				if ((*jt)->particleVec[j]->time < 8){
					//transform and draw particle: 
					particlePhi = (float)(std::rand()) / (float)(RAND_MAX / 1.5f);
					triModelToWorldMatrix = glm::translate((*jt)->particleVec[j]->position) * glm::rotate(particlePhi, particleAxis)*glm::scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
					mat4 fullTransformMatrix = worldToProjectionMatrix * triModelToWorldMatrix;
					glm::vec2 textureShift = glm::vec2(0.0f, 0.1f*(*jt)->particleVec[j]->time);
					float alpha = 1.1f - 0.1f*(*jt)->particleVec[j]->time;
					glUniform1fv(alphaUniformLocation, 1, &alpha);
					glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
					glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &triModelToWorldMatrix[0][0]);
					glUniform2fv(texShiftUniformLocation, 1, &textureShift[0]);
					glDrawElements(GL_TRIANGLES, numIndicesVec[4], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[4]));
				}
			}
			std::cout << "explosionVec.size = " << explosionVec.size() << std::endl;
			std::cout << "particleVec.size = " << (*jt)->particleVec.size() << std::endl;
			++jt;
		}
	}
}

void MeGlWindow::renderWater(mat4 worldToProjectionMatrix){
	glUseProgram(programIDwater);
	
	mat4 planeModelToWorldMatrix;

	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programIDwater,
		"modelToWorldTransformMatrix");
	GLint fullTransformUniformLocation = glGetUniformLocation(programIDwater,
		"fullTransformMatrix");

	GLint specularityUniformLocation = glGetUniformLocation(programIDwater, "specularity");
	vec3 specularity(200.0f, 200.0f, 200.0f);
	glUniform3fv(specularityUniformLocation, 1, &specularity[0]);

	GLint lightPositionUniformLocation = glGetUniformLocation(programIDwater, "lightPosition");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLint eyePositionUniformLocation = glGetUniformLocation(programIDwater, "eyePosition");
	vec3 eyePosition = camera.position;
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	//wave movement:
	GLint texMoveUniformLocation = glGetUniformLocation(programIDwater, "texMove");
	float textureMove = 0.00005f*(float)getTime();
	//textureMove = textureMove - floor(textureMove);
	glUniform1fv(texMoveUniformLocation, 1, &textureMove);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, textureIdReflect);
	glUniform1i(glGetUniformLocation(programIDwater, "ourTexture1"), 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureIdRefract);
	glUniform1i(glGetUniformLocation(programIDwater, "ourTexture2"), 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(programIDwater, "dudvMap"), 2);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(programIDwater, "normalMap"), 3);

	glBindVertexArray(VAOIdVec[3]);
	planeModelToWorldMatrix = glm::translate(vec3(10.6, waterHeight, -29.0))*glm::scale(mat4(), vec3(8.0f, 1.0f, 15.5f));
	mat4 fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[3]));
	glUseProgram(programID);
}

void MeGlWindow::renderCrystal(float ambient, mat4 worldToProjectionMatrix){
	
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");

	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programID,
		"modelToWorldTransformMatrix");

	//crystal:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[6]);
	//mat4 crystalModelToWorldMatrix = glm::translate(vec3(5.0f, 0.0f, -20.0f))*glm::scale(mat4(), vec3(0.1f, 0.1f, 0.1f))*glm::rotate(1.570796f, vec3(1.0f, 0.0f, 0.0f));
	mat4 crystalModelToWorldMatrix = mat4();
	mat4 fullTransformMatrix = worldToProjectionMatrix * crystalModelToWorldMatrix;
	vec3 ambientLight = vec3(ambient, ambient, ambient);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &crystalModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[6], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[6]));
}

void MeGlWindow::renderTerrain(float borderXpos, float borderXneg, float borderZpos, float borderZneg, float translateX, float translateZ, mat4 worldToProjectionMatrix){
	glUseProgram(programID);
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");

	GLint modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programID,
		"modelToWorldTransformMatrix");

	//plane:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 0);

	/*vec3 ambientLight = vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);*/


	glBindVertexArray(VAOIdVec[3]);
	mat4 planeModelToWorldMatrix;
	mat4 fullTransformMatrix;
	int iPos = (int)floor(borderXpos);
	int jPos = (int)floor(borderZpos);
	int iNeg = (int)floor(borderXneg);
	int jNeg = (int)floor(borderZneg);
	for (int i = iNeg; i < iPos; i++){
		for (int j = jNeg; j < jPos; j++){
			planeModelToWorldMatrix = glm::translate(vec3(translateX + i*20.0f, -0.01f, j*20.0f + translateZ))*glm::scale(vec3(10.0f, 10.0f, 10.0f));
			fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
			glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
			glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
			glDrawElements(GL_TRIANGLES, numIndicesVec[3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[3]));
		}
	}
	glUseProgram(programID);
}

void MeGlWindow::mousePressEvent(QMouseEvent *event)
{
	printf("\nMouse in board");
	setFocus(); //activates keyboard on click
}

void MeGlWindow::keyPressEvent(QKeyEvent* e){
	camera.keyUpdate(e);
	if (e->key() == Qt::Key_F){
		selectAnswerc = !selectAnswerc;
	}
	repaint();
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e){
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

// check for compile and link errors:
bool checkStatus(GLuint objectID, 
	 PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	 PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	 GLenum statusType){
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE){
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei buffersize;
		getInfoLogFunc(objectID, infoLogLength, &buffersize, buffer);

		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

// check for compile errors:
bool checkShaderStatus(GLuint shaderID){
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

// check for link errors:
bool checkProgramStatus(GLuint programID){
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

// read shader code from file:
string readShaderCode(const char* fileName){
	ifstream meInput(fileName);
	if (!meInput.good()){
		cout << "file failed to load..." << fileName;
		exit(1);
	}
	return string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

// defining the RGBA color of the vertices and fragments
// with shader txt files:
void installShaders(GLuint& ProgramId, const char* VertexShader, const char* FragmentShader){
	
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1]; // array of pointers to char variables
	string temp = readShaderCode(VertexShader);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode(FragmentShader);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)){
		return;
	}

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, vertexShaderID); //attach program to vertexshader-code
	glAttachShader(ProgramId, fragmentShaderID);//attach program to fragmentshader-code
	
	glLinkProgram(ProgramId);

	if (!checkProgramStatus(ProgramId)){
		return;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

}

void MeGlWindow::initializeGL(){
	cout << "initializeGL" << endl;
	fov = 55.0; //fieldofview angle
	setMinimumSize(700, 700);
	setMouseTracking(true);
	setFocus();
	glewInit();
	// depth buffer für z-koordinaten:
	glEnable(GL_DEPTH_TEST);
	//enable cubemaps:
	glEnable(GL_TEXTURE_CUBE_MAP);
	//render only the front sides of the triangles: 
	glEnable(GL_CULL_FACE);
	//render only aboveclipping plane:
	glEnable(GL_CLIP_DISTANCE0);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); // CounterClockWise triangles->front (rechte-Hand-Regel)
	//enable texturing with images:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthRange(0, 1);

	MeGlWindow::sendDataToOpenGL();
	MeGlWindow::sendTextureToOpenGL();
	cout << "installShaders" << endl;
	installShaders(programID, "VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	installShaders(programIDwater, "waterVertexShader.glsl", "waterFragmentShader.glsl");
	installShaders(programIDhorizBlur, "blurVertexShader.glsl", "blurFragmentShader.glsl");
	installShaders(programIDsky, "skyVertexShader.glsl", "skyFragmentShader.glsl");
	installShaders(programIDtext, "textVertexShader.glsl", "textFragmentShader.glsl");
	glUseProgram(programID);

	MeGlWindow::setupFrameBuffer();

	t1 = boost::posix_time::microsec_clock::local_time();

};

MeGlWindow::MeGlWindow(){
	QTimer * timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->setInterval(1000 / 60);
	timer->start();
}

MeGlWindow::~MeGlWindow(){
	cout << "Delete" << endl;
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
	glDeleteProgram(programIDsky);
	glDeleteProgram(programIDtext);
	glDeleteProgram(programIDwater);
	glDeleteProgram(programIDhorizBlur);
}

