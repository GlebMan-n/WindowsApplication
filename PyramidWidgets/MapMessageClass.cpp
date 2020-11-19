#include <PyramidWidgets\MapMessageClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapMessageClass::MapMessageClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(serv,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeSource(int)));
	connect(messageIsReadBut,SIGNAL(clicked()),SLOT(_slotSendResponse()));
	connect(get_track_but,SIGNAL(clicked()),SLOT(_slotLoadRoute()));
	connect(get_paletka_but,SIGNAL(clicked()),SLOT(_slotLoadGrid()));

}

MapMessageClass::~MapMessageClass()
{

}

bool MapMessageClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}
	return false;
}



void MapMessageClass::_slotSendResponse()
{
	emit signMessageIsRead(otvetMessage,_toKcps);
}

void MapMessageClass::_slotChangeSource(int index)
{
	_toKcps = index;
}

QString MapMessageClass::_parsMessage(QString message)
{
	QString result;
	QTextStream messages(&message);
	QString mes;
	while (!messages.atEnd())
	{
			mes = messages.readLine();
			int number = mes.section("$+$+",0,0).toInt();
			int type = mes.section("$+$+",1,1).toInt();
			QString mess = mes.section("$+$+",2,2);
			int id = mes.section("$+$+",3,3).toInt();
			QString name_sender = mes.section("$+$+",4,4);
			switch(type)
			{
			case 100:
				this->hide();
				result += tr("�����������: ") + name_sender  + tr("\n���������: ") + mess + tr("\n");
				get_paletka_but->setEnabled(true);
				break;
			case 101:
				this->hide();
				result += tr("�����������: ") + name_sender  + tr("\n���������: ") + mess + tr("\n");
				get_track_but->setEnabled(true);
				break;
			case 102:
				result += tr("�����������: ") + name_sender  + tr("\n���������: ���������� 102 ��������") +  tr("\n");
				break;
			case 103:
				result += tr("�����������: ") + name_sender  + tr("\n���������: ���������� 103 ��������") +  tr("\n");
				break;
			case 104:
				result += tr("�����������: ") + name_sender  + tr("\n���������: ���������� 104 ��������") +  tr("\n");
				break;
			default:
				if(!(mess.isEmpty() || mess.isNull()))
					result += tr("�����������: ") + name_sender  + tr("\n���������: ") + mess + "\n";
				break;
			}
			otvetMessage += QString::number(number)+";";
	}	


	return result;
}

void MapMessageClass::_slotResponseSended(bool sended)
{
	if(sended)
		messagePT->setPlainText(tr("������������� ��������� ����������"));
	else
		messagePT->setPlainText(tr("������ �������� ������������� ���������"));
}

void MapMessageClass::_slotLoadGrid()
{
	emit signGetGrid(_toKcps);
}

void MapMessageClass::_slotLoadRoute()
{
	emit signGetRoute(_toKcps);
}

void MapMessageClass::init(QString message)
{
	messagePT->setPlainText(_parsMessage(message));
	_loadSettings();
	serv->setCurrentIndex(_toKcps);
}

void MapMessageClass::_slotStatusRoute(QString status)
{
	if(status == "null")
		label->setText(tr("�� �������"));
	else
	{
		QFileInfo finfo(status);
		label->setText(tr("��������� ") + finfo.fileName());
		emit signDrawRoute(status);
	}
}

void MapMessageClass::_slotStatusGrid(QString status)
{
	if(status == "null")
		label->setText(tr("�� �������"));
	else
	{
		QFileInfo finfo(status);
		label->setText(tr("��������� ") + finfo.fileName());
		emit signDrawGrid(status);
	}
}