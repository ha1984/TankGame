#pragma once
#include <QtGui\qwidget.h>
#include <QtGui\qcheckbox.h>
class DebugSlider;

class MeWidget : public QWidget
{
	Q_OBJECT

	DebugSlider* zoomSlider;
	DebugSlider* speedSlider;
	QCheckBox* laserCheckBox;

private slots:
	void sliderValueChanged();
	void speedValueChanged();
	void laserValueChanged();

public:
	MeWidget();
	~MeWidget();
};

