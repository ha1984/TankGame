#pragma once
#include "ShapeGenerator.h"
#include <Vertex.h>
#include <glm\glm.hpp>
#include <vector>
#include <loadobj.h>
#include <string>
#include <tiny_obj_loader.h>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)
using namespace glm;
using namespace std;

ShapeData ShapeGenerator::makeTank(std::string objfile, int &totalvert, int &totalnorm, int &totaltext){
	ShapeData ret;

	std::string inputfile = objfile;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool retr = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!retr) {
		exit(1);
	}

	//GLushort indices[arraysize];
	//Vertex myTri[arraysize];

	//total number of vertices = 3*(number of triangles in shape 0):
	int arraysize = 3*shapes[0].mesh.num_face_vertices.size();
	ret.numVertices = arraysize;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = arraysize;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if (k < arraysize){
					ret.vertices[k].position.x = attrib.vertices[3 * (idx.vertex_index - totalvert) + 0];
					ret.vertices[k].position.y = attrib.vertices[3 * (idx.vertex_index - totalvert) + 1];
					ret.vertices[k].position.z = attrib.vertices[3 * (idx.vertex_index - totalvert) + 2];
					ret.vertices[k].normal.x = attrib.normals[3 * (idx.normal_index - totalnorm) + 0];
					ret.vertices[k].normal.y = attrib.normals[3 * (idx.normal_index - totalnorm) + 1];
					ret.vertices[k].normal.z = attrib.normals[3 * (idx.normal_index - totalnorm ) + 2];
					ret.vertices[k].texture.x = attrib.texcoords[2 * (idx.texcoord_index - totaltext) + 0];
					ret.vertices[k].texture.y = attrib.texcoords[2 * (idx.texcoord_index - totaltext) + 1];
					ret.indices[k] = (GLushort)k;
				}
				k += 1; 
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}


	totalvert += attrib.vertices.size()/3;
	totalnorm += attrib.normals.size()/3;
	totaltext += attrib.texcoords.size()/2;

	//std::memcpy(ret.vertices, myTri, sizeof(myTri));

	//std::memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makeObject(std::string objfile){
	ShapeData ret;

	std::string inputfile = objfile;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool retr = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!retr) {
		exit(1);
	}

	//GLushort indices[arraysize];
	//Vertex myTri[arraysize];

	//total number of vertices = 3*(number of triangles in shape 0):
	int arraysize = 0;
	for (int i = 0; i < shapes.size(); i++){
		arraysize += 4 * shapes[i].mesh.num_face_vertices.size();
	}
	ret.numVertices = arraysize;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = arraysize;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
	//for (size_t s = 0; s < 2; s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if (k < arraysize){
					ret.vertices[k].position.x = attrib.vertices[3 * idx.vertex_index + 0];
					ret.vertices[k].position.y = attrib.vertices[3 * idx.vertex_index + 1];
					ret.vertices[k].position.z = attrib.vertices[3 * idx.vertex_index + 2];
					ret.vertices[k].normal.x = attrib.normals[3 * idx.normal_index + 0];
					ret.vertices[k].normal.y = attrib.normals[3 * idx.normal_index + 1];
					ret.vertices[k].normal.z = attrib.normals[3 * idx.normal_index + 2];
					ret.vertices[k].texture.x = attrib.texcoords[2 * idx.texcoord_index + 0];
					ret.vertices[k].texture.y = attrib.texcoords[2 * idx.texcoord_index + 1];
					ret.indices[k] = (GLushort)k;
				}
				k += 1;
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	//std::memcpy(ret.vertices, myTri, sizeof(myTri));

	//std::memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makePlane(int dimensions){
	ShapeData ret;

	ret.numVertices = 6*dimensions*dimensions;
	ret.vertices = new Vertex[ret.numVertices];
	
	ret.numindices = 6*dimensions*dimensions;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	//int half = dimensions / 2;
	int half = 0;
	for (int i = 0; i < dimensions; i++){
		for (int j = 0; j < dimensions; j++){
			Vertex& thisVert0 = ret.vertices[6*(i*dimensions + j) + 0];
			thisVert0.position = vec3((j - half)*2.0f, +0.0f,(i - half)*2.0f);
			thisVert0.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert0.texture = vec2(0.0f, 0.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert1 = ret.vertices[6 * (i*dimensions + j) + 1];
			thisVert1.position = vec3((j - half)*2.0f, +0.0f, +2.0f + (i - half)*2.0f);
			thisVert1.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert1.texture = vec2(0.0f, 1.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert2 = ret.vertices[6 * (i*dimensions + j) + 2];
			thisVert2.position = vec3(+2.0f + (j - half)*2.0f, +0.0f, +2.0f + (i - half)*2.0f);
			thisVert2.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert2.texture = vec2(1.0f, 1.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert3 = ret.vertices[6 * (i*dimensions + j) + 3];
			thisVert3.position = vec3(+2.0f + (j - half)*2.0f, +0.0f, +2.0f + (i - half)*2.0f);
			thisVert3.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert3.texture = vec2(1.0f, 1.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert4 = ret.vertices[6 * (i*dimensions + j) + 4];
			thisVert4.position = vec3(+2.0f + (j - half)*2.0f, +0.0f,(i - half)*2.0f);
			thisVert4.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert4.texture = vec2(+1.0f, 0.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert5 = ret.vertices[6 * (i*dimensions + j) + 5];
			thisVert5.position = vec3((j - half)*2.0f, +0.0f,(i - half)*2.0f);
			thisVert5.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert5.texture = vec2(0.0f, 0.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;
		}
	}

	return ret;
}

ShapeData ShapeGenerator::makeLine(float length){
	ShapeData ret;

	Vertex myTri[] =
	{
		vec3(-0.03f, -0.03f, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.4f, 0.0f),

		vec3(-0.03f, length, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.45f, 0.1f),

		vec3(+0.03f, -0.03f, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.5f, 0.0f),

		vec3(-0.03f, length, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.45f, 0.1f),

		vec3(+0.03f, length, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.45f, 0.1f),

		vec3(+0.03f, -0.03f, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.5f, 0.0f),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = { 0, 1, 2 , 3, 4, 5};
	ret.numindices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numindices];

	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makeTriangle(){
	ShapeData ret;

	Vertex myTri[] =
	{
		vec3(-1.0f, -1.0f, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.4f,0.0f),

		vec3(+0.0f, +1.0f, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.45f, 0.1f),

		vec3(+1.0f, -1.0f, +0.0f),
		vec3(+0.0f, +0.0f, +1.0f),
		vec2(0.5f, 0.0f),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = { 0, 1, 2 };
	ret.numindices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numindices];

	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makeCube(){
	ShapeData ret;

	Vertex myTri[] =
	{
		vec3(-1.0f, +1.0f, +1.0f), // 0
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(0.0f,1.0f),
		vec3(+1.0f, +1.0f, +1.0f), // 1
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(1.0f,1.0f),
		vec3(+1.0f, +1.0f, -1.0f), // 2
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(1.0f, 0.0f),
		vec3(-1.0f, +1.0f, -1.0f), // 3
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(0.0f, 0.0f),

		vec3(-1.0f, +1.0f, -1.0f), // 4
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(0.0f, 1.0f),
		vec3(+1.0f, +1.0f, -1.0f), // 5
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(1.0f, 1.0f),
		vec3(+1.0f, -1.0f, -1.0f), // 6
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(1.0f, 0.0f),
		vec3(-1.0f, -1.0f, -1.0f), // 7
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(0.0f, 0.0f),

		vec3(+1.0f, +1.0f, -1.0f), // 8
		vec3(+1.0f, +0.0f, +0.0f), // normal
		vec2(0.0f, 1.0f),
		vec3(+1.0f, +1.0f, +1.0f), // 9
		vec3(+1.0f, +0.0f, +0.0f), // normal
		vec2(1.0f, 1.0f),
		vec3(+1.0f, -1.0f, +1.0f), // 10
		vec3(+1.0f, +0.0f, +0.0f), // normal
		vec2(1.0f, 0.0f),
		vec3(+1.0f, -1.0f, -1.0f), // 11
		vec3(+1.0f, +0.0f, +0.0f), // normal
		vec2(0.0f, 0.0f),

		vec3(-1.0f, +1.0f, +1.0f), // 12
		vec3(-1.0f, +0.0f, +0.0f), // normal
		vec2(0.0f, 1.0f),
		vec3(-1.0f, +1.0f, -1.0f), // 13
		vec3(-1.0f, +0.0f, +0.0f), // normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f), // 14
		vec3(-1.0f, +0.0f, +0.0f), // normal
		vec2(1.0f, 0.0f),
		vec3(-1.0f, -1.0f, +1.0f), // 15
		vec3(-1.0f, +0.0f, +0.0f), // normal
		vec2(0.0f, 0.0f),

		vec3(+1.0f, +1.0f, +1.0f), // 16
		vec3(+0.0f, +0.0f, +1.0f), // normal
		vec2(0.0f, 1.0f),
		vec3(-1.0f, +1.0f, +1.0f), // 17
		vec3(+0.0f, +0.0f, +1.0f), // normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, -1.0f, +1.0f), // 18
		vec3(+0.0f, +0.0f, +1.0f), // normal
		vec2(1.0f, 0.0f),
		vec3(+1.0f, -1.0f, +1.0f), // 19
		vec3(+0.0f, +0.0f, +1.0f), // normal
		vec2(0.0f, 0.0f),

		vec3(+1.0f, -1.0f, -1.0f), // 20
		vec3(+0.0f, -1.0f, +0.0f), // normal
		vec2(0.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f), // 21
		vec3(+0.0f, -1.0f, +0.0f), // normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, -1.0f, +1.0f), // 22
		vec3(+0.0f, -1.0f, +0.0f), // normal
		vec2(1.0f, 0.0f),
		vec3(+1.0f, -1.0f, +1.0f), // 23
		vec3(+0.0f, -1.0f, +0.0f), // normal
		vec2(0.0f, 0.0f),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	ret.numindices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numindices];

	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makeQuad(float width, vector<float> texCoord){
	ShapeData ret;

	ret.numVertices = 6;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = 6;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	//int half = dimensions / 2;
	int half = 0;
	
			Vertex& thisVert0 = ret.vertices[0];
			thisVert0.position = vec3(-0.5f, 0.4375f, +0.0f);
			thisVert0.normal = vec3(+0.0f, +0.0f, +1.0f);
			thisVert0.texture = vec2(texCoord[0], texCoord[1]);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert1 = ret.vertices[1];
			thisVert1.position = vec3(-0.5f + width, +0.5f, +0.0f);
			thisVert1.normal = vec3(+0.0f, +0.0f, +1.0f);
			thisVert1.texture = vec2(texCoord[4], texCoord[5]);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert2 = ret.vertices[2];
			thisVert2.position = vec3(-0.5f, 0.5f, +0.0f);
			thisVert2.normal = vec3(+0.0f, +0.0f, +1.0f);
			thisVert2.texture = vec2(texCoord[2], texCoord[3]); 
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert3 = ret.vertices[3];
			thisVert3.position = vec3(-0.5f + width, +0.5f, +0.0f);
			thisVert3.normal = vec3(+0.0f, +0.0f, +1.0f);
			thisVert3.texture = vec2(texCoord[4], texCoord[5]);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert4 = ret.vertices[4];
			thisVert4.position = vec3(-0.5f, 0.4375f, +0.0f);
			thisVert4.normal = vec3(+0.0f, +0.0f, +1.0f);
			thisVert4.texture = vec2(texCoord[0], texCoord[1]); 
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert5 = ret.vertices[5];
			thisVert5.position = vec3(-0.5f + width, 0.4375f, +0.0f);
			thisVert5.normal = vec3(+0.0f, +0.0f, +1.0f);
			thisVert5.texture = vec2(texCoord[6], texCoord[7]);
			ret.indices[k] = (GLushort)k;
			k += 1;
		
	return ret;
}

ShapeData ShapeGenerator::makeBox(float width, vector<float> texCoord){
	ShapeData ret;

	ret.numVertices = 6;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = 6;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	//int half = dimensions / 2;
	int half = 0;

	Vertex& thisVert0 = ret.vertices[0];
	thisVert0.position = vec3(-0.5f, 0.15f, +0.0f);
	thisVert0.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert0.texture = vec2(texCoord[0], texCoord[1]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert1 = ret.vertices[1];
	thisVert1.position = vec3(-0.5f + width, +0.5f, +0.0f);
	thisVert1.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert1.texture = vec2(texCoord[4], texCoord[5]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert2 = ret.vertices[2];
	thisVert2.position = vec3(-0.5f, 0.5f, +0.0f);
	thisVert2.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert2.texture = vec2(texCoord[2], texCoord[3]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert3 = ret.vertices[3];
	thisVert3.position = vec3(-0.5f + width, +0.5f, +0.0f);
	thisVert3.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert3.texture = vec2(texCoord[4], texCoord[5]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert4 = ret.vertices[4];
	thisVert4.position = vec3(-0.5f, 0.15f, +0.0f);
	thisVert4.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert4.texture = vec2(texCoord[0], texCoord[1]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert5 = ret.vertices[5];
	thisVert5.position = vec3(-0.5f + width, 0.15f, +0.0f);
	thisVert5.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert5.texture = vec2(texCoord[6], texCoord[7]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	return ret;
}