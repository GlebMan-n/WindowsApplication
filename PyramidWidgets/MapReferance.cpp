#include <PyramidWidgets\MapReferance.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapReferanceClass::MapReferanceClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	_currentFileIndex = 0;
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(nextFile,SIGNAL(clicked()),SLOT(_nextRef()));
	connect(pervFile,SIGNAL(clicked()),SLOT(_pervRef()));

}

MapReferanceClass::~MapReferanceClass()
{
}

void MapReferanceClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}



void MapReferanceClass::setPathList(QFileInfoList fileInfo)
{
	_fileInfo = fileInfo;
}
void MapReferanceClass::_nextRef()
{
	int maxIndex = _fileInfo.size()-1;
	if(maxIndex < 0)
		return;

	if(_currentFileIndex+1 > maxIndex)
		_currentFileIndex = 0;
	else
		_currentFileIndex+=1;
	_showCurrentFile();
	
}

void MapReferanceClass::_pervRef()
{
	int maxIndex = _fileInfo.size()-1;
	if(maxIndex < 0)
		return;

	if(_currentFileIndex-1 < 0)
		_currentFileIndex = maxIndex;
	else
		_currentFileIndex--;
	_showCurrentFile();
}

void MapReferanceClass::_showCurrentFile()
{
	QString tmp;
	QString temp(_fileInfo.at(_currentFileIndex).absoluteFilePath());
	QFile file(temp);
	if (!file.exists())
	{
		reference->setPlainText(tr("������\n����� ������� ���"));
		return ;
	}

	if(!file.open(QIODevice::ReadOnly))
	{
		reference->setPlainText(tr("������\n�� ���� ������� ���� �������"));
		return ;
	}
	
	QTextStream in(&file);
	while (!in.atEnd()) 
	 tmp += in.readLine() + tr("\n");         

	if(tmp.isEmpty() || tmp.isNull())
	{
		reference->setPlainText(tr("������\n���� ������� ������"));
		return;
	}
	QString tmp2(_fileInfo.at(_currentFileIndex).baseName());
	if(tmp2 == "ref102")
		header->setText(tr("������� ������"));

	if(tmp2 == "ref103")
		header->setText(tr("������ ��������"));

	if(tmp2 == "ref104")
		header->setText(tr("������ ��������"));

	
	
	reference->setPlainText(tmp);
}

void MapReferanceClass::init()
{
	_showCurrentFile();
	this->show();
}