#include "MeWidget.h"
#include <Qt\qdebug.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <MeGlWindow.h>
#include <DebugSlider.h>

extern MeGlWindow * meWindow;

MeWidget::MeWidget()
{
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	QVBoxLayout* controlsLayout;
	mainLayout->addLayout(controlsLayout = new QVBoxLayout);
	
	mainLayout->addWidget(meWindow);

	QHBoxLayout* zoomLaserLayout;
	controlsLayout->addLayout(zoomLaserLayout = new QHBoxLayout);
	zoomLaserLayout->addWidget(zoomSlider = new DebugSlider(54.5f, 54.5f, 55.0f, true, 50.0, "Zoom:"));
	zoomLaserLayout->addWidget(speedSlider = new DebugSlider(0.1f, 0.1f, 1.5f, true, 50.0, "Speed:"));
	zoomLaserLayout->addWidget(laserCheckBox = new QCheckBox);

	QString laserText = QString("Laser");
	laserCheckBox->setText(laserText);

	connect(zoomSlider, SIGNAL(valueChanged(float)), this, SLOT(sliderValueChanged()));
	connect(speedSlider, SIGNAL(valueChanged(float)), this, SLOT(speedValueChanged()));
	connect(laserCheckBox, SIGNAL(stateChanged(int)), this, SLOT(laserValueChanged()));

}

void MeWidget::sliderValueChanged(){
	//qDebug() << "slider value changed";
	meWindow->fov = zoomSlider->value();
	meWindow->repaint();
}

void MeWidget::speedValueChanged(){
	//qDebug() << "slider value changed";
	meWindow->speed = speedSlider->value();
	meWindow->repaint();
}

void MeWidget::laserValueChanged(){
	//qDebug() << "slider value changed";
	meWindow->laserOn = !meWindow->laserOn;
	meWindow->repaint();
}


MeWidget::~MeWidget()
{
}
