#include <PyramidWidgets\MapMarkEdit.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapMarkEdit::MapMarkEdit(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	_parent = parent;
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_currentIndex = 0;
	_pathToMark = "null";
	if(_loadSettings())
		_loadMarksFromFile(_pathToMark);

	connect(this->perv,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->next,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->save,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->cancel,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->del,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->keyboard,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->perv,SIGNAL(clicked()),SLOT(_slotPervMark()));
	connect(this->next,SIGNAL(clicked()),SLOT(_slotNextMark()));
	connect(this->save,SIGNAL(clicked()),SLOT(_slotSave()));
	connect(this->cancel,SIGNAL(clicked()),SLOT(_slotCancel()));
	connect(this->del,SIGNAL(clicked()),SLOT(_slotDelete()));
	connect(this->keyboard,SIGNAL(clicked()),SLOT(_slotKeyboard()));

	_showMarkData(_currentIndex);
}

MapMarkEdit::~MapMarkEdit()
{

}

bool MapMarkEdit::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";

	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToMark = akps_settings.value("programm/path_to_marks").toString();
		_pathToMark += "\\marks.file";
		return true;
	}
	return false;
}

bool MapMarkEdit::_saveMarksToFile(QString path)
{
	//формат файла отметок
	//тип,дата,время,описание,
	//координаты,высота, marks\n
	QFile fileMarks(_pathToMark);
	fileMarks.remove();
	if(!fileMarks.open(QIODevice::Append | QIODevice::Text)) 
		return false;

	QTextStream out(&fileMarks);

	for(int i = 0; i < marks.size(); i++)
	{
		QString string = marks.at(i).type + "," + marks.at(i).date + "," + marks.at(i).time + "," + 
					 marks.at(i).info +"," + marks.at(i).lat + "," + marks.at(i).lon + "," + marks.at(i).height + ",marks\n";
		out << string;		
	}

	fileMarks.close();
	return true;
}

bool MapMarkEdit::_loadMarksFromFile(QString path)
{
	QFile markFile(_pathToMark);
	if(!markFile.exists ())
		return false;

	if (!markFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream markStream;
	markStream.setDevice(&markFile);
	marks.clear();
	while (!markStream.atEnd())
	{
		QString str = markStream.readLine();
		_addMarkToVector(str);					
	}
	markFile.close();
	return true;
}

void MapMarkEdit::_addMarkToVector(QString str)
{
	if(str.section(',', 7, 7) == "marks")
	{
		MARK mark;
		mark.type = str.section(',', 0, 0);
		mark.date = str.section(',', 1, 1);
		mark.time = str.section(',', 2, 2);
		mark.info = str.section(',', 3, 3);
		mark.lat = str.section(',', 4, 4);
		mark.lon = str.section(',', 5, 5);
		mark.height = str.section(',', 6, 6);
		marks.push_back(mark);
	}
}

void MapMarkEdit::_showMarkData(short index)
{

	if(marks.size() == 0)
	{
		description->setPlainText(tr("отметок нет"));
		info->setText(tr("отметок нет"));
		status->setText(QString::number(0) + tr(" из ")+ QString::number(0));

		return;
	}

	if(index < 0)
		return;

	if(index > marks.size())
		return;
	
	status->setText(QString::number(index+1) + tr(" из ")+ QString::number(marks.size()));
	
	//тип
	if(marks.at(index).type == "100")
	{
		//находим индекс для прочего и устанавливаем его
		typeComboBox->setCurrentIndex(typeComboBox->findText(tr("Прочее")));
	}
	else
	{
		//устанавливаем индекс из файла минус 1
		int type_ = marks.at(index).type.toInt();
		typeComboBox->setCurrentIndex(type_ - 1);
	}

	info->setText(tr("дата:") + marks.at(index).date 
		+ tr("\nвремя: ") + marks.at(index).time
		+ tr("\nдолгота: ") + CalcFunctions::dDToDDMMSS(marks.at(index).lon.toDouble())
		+ tr("\nширота: ") + CalcFunctions::dDToDDMMSS(marks.at(index).lat.toDouble())
		+ tr("\nвысота: ") + marks.at(index).height);
	
	//описание
	description->setPlainText(marks.at(index).info);
}

void MapMarkEdit::_slotKeyboard()
{
	CalcFunctions::runKeyboard(this);
}

short MapMarkEdit::_nextMark()
{
	if(marks.size() == 0)
	{
		status->setText(QString::number(0) + tr(" из ")+ QString::number(0));
		description->setPlainText(tr("отметок нет"));
		info->setText(tr("отметок нет"));
		return 0;
	}
	if(_currentIndex+1 < marks.size() )
	{
		_saveChangesToVector();
		_currentIndex++;		
		return _currentIndex;
	}
	else
	{
		_saveChangesToVector();
		return _currentIndex;
	}

}

short MapMarkEdit::_pervMark()
{
	if(marks.size() == 0)
	{
		status->setText(QString::number(0) + tr(" из ")+ QString::number(0));
		description->setPlainText(tr("отметок нет"));
		info->setText(tr("отметок нет"));
		return 0;
	}
	if(_currentIndex > 0)
	{
		_saveChangesToVector();
		_currentIndex--;		
		return _currentIndex;
	}
	else
	{
		_saveChangesToVector();
		return _currentIndex;
	}
}

void MapMarkEdit::_cancel()
{
	 _loadMarksFromFile(_pathToMark);
	 _showMarkData(_currentIndex);
}

bool MapMarkEdit::_save()
{
	if(marks.size() == 0)
	{
		status->setText(QString::number(0) + tr(" из ")+ QString::number(0));
		description->setPlainText(tr("отметок нет"));
		info->setText(tr("отметок нет"));
		return false;
	}
	_saveChangesToVector();
	_saveMarksToFile(_pathToMark);
	emit signUpdateMarks();
	return true;
}

bool MapMarkEdit::_deleteCurrentMark()
{
	if(marks.size() == 0)
	{
		status->setText(QString::number(0) + tr(" из ")+ QString::number(0));
		description->setPlainText(tr("отметок нет"));
		info->setText(tr("отметок нет"));
		return true;
	}
	this->hide();
	SelectDialog dlgDelMark(_parent,tr("Трек"),
	tr("<br>Удалить отметку?</br>"));
		dlgDelMark.exec();
	this->show();
	if(dlgDelMark.result == 0)
		return false;

	short i = _currentIndex;
	marks.erase(marks.begin() + i);

	if(marks.size() == 0)
	{
		status->setText(QString::number(0) + tr(" из ")+ QString::number(0));
		description->setPlainText(tr("отметок нет"));
		info->setText(tr("отметок нет"));
		_saveMarksToFile(_pathToMark);
		_loadMarksFromFile(_pathToMark);
		_showMarkData(_currentIndex);
		emit signUpdateMarks();
		return true;
	}

	if(_currentIndex > 0)
		_currentIndex--;
	

	_saveMarksToFile(_pathToMark);
	_loadMarksFromFile(_pathToMark);
	_showMarkData(_currentIndex);
	emit signUpdateMarks();
	return true;
}

void MapMarkEdit::_slotNextMark()
{
	_showMarkData(_nextMark());
}

void MapMarkEdit::_slotPervMark()
{
	_showMarkData(_pervMark());
}

void MapMarkEdit::_slotSave()
{
	_save();
}

void MapMarkEdit::_slotCancel()
{
	_cancel();
}

bool MapMarkEdit::_saveChangesToVector()
{
	marks.at(_currentIndex).info = description->toPlainText();
	if(typeComboBox->currentText() == tr("Прочее"))
		marks.at(_currentIndex).type  = "100";
	else
		marks.at(_currentIndex).type = QString::number(typeComboBox->currentIndex() + 1);
	return true;
}

void MapMarkEdit::_slotDelete()
{
	_deleteCurrentMark();
}