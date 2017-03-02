#ifndef DEBUG_SLIDER_H
#define DEBUG_SLIDER_H
#include <QtGui\qwidget>
class QSlider;
class QLabel;

class DebugSlider : public QWidget
{
	Q_OBJECT

	QSlider* slider;
	QLabel* label;
	QLabel* text;
	float sliderGranularity;
	float min;
	float max;

private slots:
	void sliderValueChanged();

signals:
		void valueChanged(float newValue);

public:
	DebugSlider(float value,
		float min, float max, 
		bool textOnLeft, float granularity, const char* name);
	float value() const;
	void setValue(float newValue);
};

#endif