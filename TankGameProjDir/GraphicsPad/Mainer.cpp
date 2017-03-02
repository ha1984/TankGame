#pragma once
#include <Qt\qapplication.h>
#include <MeGlWindow.h>
#include <MeWidget.h>

MeGlWindow * meWindow;

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	meWindow = new MeGlWindow();
	MeWidget widg;
	widg.show();
	
	//meWindow->resize(700, 700);
	//meWindow->show();

	return app.exec();
}