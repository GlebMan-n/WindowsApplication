#include <PyramidWidgets\GeneralSignClass.h>
#include <QWebFrame>
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

SignClass::SignClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	
	_pathPhotosSigns = "null";
	_fullPathToSigns = "null";
	_signsPhotoExp = NULL;
	connect(this->back_but,SIGNAL(clicked()),this,SLOT(_slotBack()));
	connect(this->right_photo_but,SIGNAL(clicked()),this,SLOT(_slotNextPhoto()));
	connect(this->left_photo_but,SIGNAL(clicked()),this,SLOT(_slotPervPhoto()));
	connect(this->upBut,SIGNAL(clicked()),this,SLOT(_slotUpText()));
	connect(this->downBut,SIGNAL(clicked()),this,SLOT(_slotDownText()));
	
	connect(this->back_but,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->right_photo_but,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->left_photo_but,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->upBut,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->downBut,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	_initSigns();
}


void SignClass::_slotBack()
{
	this->close();
}

void SignClass::_initSigns()
{
	if(_loadSettings())
	{
		//приметы фотографии
		_signsPhotoExp = new FileExplorerClass(this,_pathPhotosSigns,QStringList() << "*.jpg" );
		photo_lbl->setPixmap(QPixmap(_signsPhotoExp->getCurrentFile(true)));
		lblNumbPhoto->setText(QString::number(_signsPhotoExp->getCurrentIndex()) + tr(" из ")+QString::number(_signsPhotoExp->getCountOfFiles()));
		if(_signsPhotoExp->getCountOfFiles()<1)
			lblNumbPhoto->setText(tr("файлов нет"));
		//приметы htm
		QFile file(_fullPathToSigns);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
		else
		{
			file.close();
			this->omensEdit->setUrl(_fullPathToSigns);
		}	
	}

}

bool SignClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathPhotosSigns = akps_settings.value("programm/path_to_signs").toString();
		_fullPathToSigns = _pathPhotosSigns + "signs.htm";
		return true;
	}
	return false;
}

void SignClass::_slotNextPhoto()
{
	if(_signsPhotoExp->getCountOfFiles()<1)
		lblNumbPhoto->setText(tr("файлов нет"));
	else
	{
		photo_lbl->setPixmap(QPixmap(_signsPhotoExp->getNextFile(true)));
		lblNumbPhoto->setText(QString::number(_signsPhotoExp->getCurrentIndex()) + tr(" из ")+QString::number(_signsPhotoExp->getCountOfFiles()));
	}
}

void SignClass::_slotPervPhoto()
{
	if(_signsPhotoExp->getCountOfFiles()<1)
		lblNumbPhoto->setText(tr("файлов нет"));
	else
	{
		photo_lbl->setPixmap(QPixmap(_signsPhotoExp->getPervFile(true)));
		lblNumbPhoto->setText(QString::number(_signsPhotoExp->getCurrentIndex()) + tr(" из ")+QString::number(_signsPhotoExp->getCountOfFiles()));
	}
}

void SignClass::_slotUpText()
{
	this->omensEdit->page()->mainFrame()->setScrollBarValue (Qt::Vertical,this->omensEdit->page()->mainFrame()->scrollBarValue(Qt::Vertical) + 25);
}
void SignClass::_slotDownText()
{
	this->omensEdit->page()->mainFrame()->setScrollBarValue (Qt::Vertical,this->omensEdit->page()->mainFrame()->scrollBarValue(Qt::Vertical) - 25);

}

void SignClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}