#include "stdafx.h"
#include <PyramidM\MSVC_2008\LineDialogClass.h>

LineDialogClass::LineDialogClass(QWidget *parent,short x/*������������ ���� �� �*/, short y/*������������ ���� �� �*/)
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

	
		//���������
		int kmP = pointM /1000;
		//�����
		int mP = pointM - ((int)pointM/1000)*1000;

		//������������� �������� � ������
		this->pointKm->display(kmP);//minLat->display(val);
		this->pointM->display(mP);
		this->pointDegr->display(pointNapr);
}