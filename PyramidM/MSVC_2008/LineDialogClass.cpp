#include "stdafx.h"
#include <PyramidM\MSVC_2008\LineDialogClass.h>

LineDialogClass::LineDialogClass(QWidget *parent,short x/*расположение окна по ’*/, short y/*расположение окна по ”*/)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(x,y,540,180);
}

void LineDialogClass::setLenLineData(int pointM, short pointNapr)
{
	if (pointNapr>-361 && pointNapr<361)
	{

	}
	else
		pointNapr = 0;

	
		//километры
		int kmP = pointM /1000;
		//метры
		int mP = pointM - ((int)pointM/1000)*1000;

		//устанавливаем значени€ в диалог
		this->pointKm->display(kmP);//minLat->display(val);
		this->pointM->display(mP);
		this->pointDegr->display(pointNapr);
}