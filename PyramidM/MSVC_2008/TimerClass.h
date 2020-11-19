#include "stdafx.h"
#include <QThread>

class TimerClass : public QThread
{
	Q_OBJECT
public:
	TimerClass();
	TimerClass(bool b_min);
	~TimerClass();


	void stop();
	int getTime();
	bool stopped;
	void setTime(int Time);
protected:
	void run();


private:
	int min;
	bool _min;

	
};