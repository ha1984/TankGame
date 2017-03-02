#include "textbox.h"
#include <iostream>
#include <string>
#include <vector>
#include <MeGlWindow.h>
using namespace std;

extern MeGlWindow * meWindow;

textbox::textbox(const char* str)
{
	//convert text string to vector of character integers b:
	len = strlen(str); //len = anzahl buchstaben inkl. Leerzeichen
	//read in width of b[i]-th character from csv:

	//float width = 0.0625f;
	float curs = 0;
	shapeIdx.resize(len);
	cursor.resize(len);
	for (int i = 0; i < len; i++){
		shapeIdx[i] = (int)str[i];
		cursor[i] = curs;
		curs = cursor[i] + (float)meWindow->csvWidths[shapeIdx[i]] * 2.0f / (float)meWindow->width();
		//std::cout << shapeIdx[i] << std::endl;
	}	

	//then render ShapeDataVec[7+ shapeIdx[i]] in drawing()

	//calculate 4 vec2 with texcoords for every character:

	//calculate 4 vec2 with vertex-positions for every character (size of quad depends on width of character):


}

textbox::~textbox()
{
}
