#include "stdafx.h"
#include "ui_DialogLenLineDlg.h"

class LineDialogClass : public QDialog, public Ui::LenLineDlg
{
	Q_OBJECT
public:
	LineDialogClass(QWidget *parent,short x/*расположение окна по Х*/, short y/*расположение окна по У*/);
	void setLenLineData(int pointM, short pointNapr);  
};