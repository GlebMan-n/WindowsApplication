#include "stdafx.h"
#include <MSVC_2008\TimerClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

void TimerClass::run()
{
	stopped = false;
	while(!stopped)
	{
		if(_min)
			msleep(60000);
		else
			msleep(1000);

		min +=1;
	}
	stopped = true;
}

TimerClass::TimerClass()
{
	stopped = true;
	min = 0;
	_min = true;

}

TimerClass::TimerClass(bool b_min)
{
	stopped = true;
	min = 0;
	_min = b_min;
}

TimerClass::~TimerClass()
{

}	

void TimerClass::stop()
{
	stopped = true;
}

int TimerClass::getTime()
{
	return min;
}

void TimerClass::setTime(int Time)
{
	try
	{
		this->stop();
		while(!this->isFinished())
		{
			this->msleep(10);
		}
		min = Time;
		this->start();
	}
	catch(...)
	{
		CalcFunctions::logError("TimerClass::setTime");
	}
}