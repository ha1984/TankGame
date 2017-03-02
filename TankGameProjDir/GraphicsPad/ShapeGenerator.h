#pragma once
#include <ShapeData.h>
#include <string>
#include <vector>
class ShapeGenerator
{
public:
	static ShapeData makePlane(int dimensions);
	static ShapeData makeTank(std::string objfile, int &totalvert, int &totalnorm, int &totaltext);
	static ShapeData makeObject(std::string objfile);
	static ShapeData makeLine(float length);
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static ShapeData makeQuad(float width, std::vector<float> texCoord);
	static ShapeData makeBox(float width, std::vector<float> texCoord);
};

