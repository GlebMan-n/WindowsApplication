#include <PyramidWidgets\PsoExplorerClass.h>
#include <QSettings>
#include <PyramidWidgets\FileExplorerClass.h>

PsoExplorerClass::PsoExplorerClass(QObject* parent)
{
	if(!_loadSettings())
		_saveSettings();
	_listOfPSO = _getListOfPso(_pathToPso);
	
	if(_pathToPso == "NULL" || _pathToPso.isEmpty() || _pathToPso == "0")
		_pathToPso = QCoreApplication::applicationDirPath ()+"\\tmp\\psoBackup\\";

	if(_currentPsoName == "NULL" || _currentPsoName.isEmpty() || _currentPsoName == "0")
		createPso();
}

PsoExplorerClass::~PsoExplorerClass()
{
	_saveSettings();
}

QDate PsoExplorerClass::getDateOfCurrentPso()
{
	QDate result;
	result = QDate::fromString(_currentPsoName.left(8),"ddMMyyyy");
	return result;
}

QTime PsoExplorerClass::getTimeOfCurrentPso()
{
	QTime result;
	result = QTime::fromString(_currentPsoName.right(4),"hhmm");
	return result;
}


bool PsoExplorerClass::_delPso(QString nameOfPso)
{

	return true;
}


QStringList PsoExplorerClass::_getListOfPso(QString pathToPso)
{
	QStringList result;
	result.clear();
	QDir dir(pathToPso);

	if(dir.exists())
		result = dir.entryList(QDir::AllDirs);

	if(result.size() > 0)
	{
		if(result.at(0) == ".")
			result.removeFirst();

		if(result.at(0) == "..")
			result.removeFirst();
	}
	return result;
}

bool PsoExplorerClass::_erasePsoData()
{
	return true;
}


bool PsoExplorerClass::_restorePso(QString nameOfPso)
{
	bool res = false;
	QString pathToData = QCoreApplication::applicationDirPath () + "\\data\\";
	QString pathToPsoBackup(_pathToPso+nameOfPso+"\\");
	
	//Проверяем наличие директории для заданного имени ПСО
	QDir dir(_pathToPso+nameOfPso);
	
	if(!dir.exists())
		return false;
	//копируем текущие данные о ПСО в папку ПСО
	if(!backupPso())
		return false;

	//удаляем текущие данные
	_erasePsoData();
	
	//восстанавливаем данные ПСО из бекапа
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\marks\\", pathToMarks);
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\mroute\\",pathToMRoute);
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\video\\",pathToVideo);
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\photo\\",pathToPhoto);
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\route\\",pathToRoute);
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\grid\\",pathToGrid);
	res = FileExplorerClass::copyAllFilesFromDir(pathToPsoBackup+"\\pso\\",pathToData+"\\pso\\");

	//приравниваем имя текущего ПСО к имени восстановленого ПСО
	_currentPsoName = nameOfPso;
	return res;
}

QString PsoExplorerClass::_createNewPso(QString pathToPso)
{
	QString result(QDate::currentDate().toString("ddMMyyyy")+QTime::currentTime().toString("hhmm"));
	
	QDir dir;
	bool ok = dir.mkpath(pathToPso+result);
	if(!ok) 
		return "ERROR";
	_currentPsoName = result;
	_saveSettings();
	return result;
}

void PsoExplorerClass::_slotDelPso(QString nameOfPso)
{
	_delPso(nameOfPso);
	emit signCurrentPso(_currentPsoName);
}

void PsoExplorerClass::_slotRestorePso(QString nameOfPso)
{
	_restorePso(nameOfPso);
	emit signCurrentPso(_currentPsoName);
}

void PsoExplorerClass::_slotNewPso()
{
	createPso();
	emit signCurrentPso(_currentPsoName);
}

bool PsoExplorerClass::_copyCurrentPso()
{
	bool res = false;
	QString pathToData = QCoreApplication::applicationDirPath () + "\\data\\";
	QString pathToPsoBackup(_pathToPso+_currentPsoName+"\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToTracks,pathToPsoBackup+"\\tracks\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToMarks,pathToPsoBackup+"\\marks\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToMRoute,pathToPsoBackup+"\\mroute\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToVideo,pathToPsoBackup+"\\video\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToPhoto,pathToPsoBackup+"\\photo\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToRoute,pathToPsoBackup+"\\route\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToGrid,pathToPsoBackup+"\\grid\\");
	res = FileExplorerClass::copyAllFilesFromDir(pathToData+"\\pso\\",pathToPsoBackup+"\\pso\\");

	_saveSettings();
	return true;
}

bool PsoExplorerClass::backupPso()
{
	return _copyCurrentPso();
}

bool PsoExplorerClass::createPso()
{
	return (_createNewPso(_pathToPso) != "NULL");
}

bool PsoExplorerClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_currentPsoName = akps_settings.value("pso/nameOfCurrentPso").toString();
		_pathToPso = akps_settings.value("pso/pathToPso").toString();
		pathToMarks = akps_settings.value("programm/path_to_marks").toString();
		pathToMRoute = akps_settings.value("programm/path_to_mroute").toString();
		pathToVideo =  akps_settings.value("programm/path_to_video").toString();
		pathToPhoto = akps_settings.value("programm/path_to_photo").toString();
		pathToTracks = akps_settings.value("programm/path_to_track").toString();
		pathToRoute = akps_settings.value("programm/path_to_route").toString();
		pathToGrid = akps_settings.value("programm/path_to_grid").toString();
	}	
	return true;
}

bool PsoExplorerClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	akps_settings.setValue("pso/nameOfCurrentPso", _currentPsoName);
	akps_settings.setValue("pso/pathToPso", _pathToPso);
	akps_settings.sync();
	return true;
}

QString PsoExplorerClass::getNameOfCurrentPso()
{
	return _currentPsoName;
}

//Функция удаления папки
bool PsoExplorerClass::removeFolder(QDir & dir)
{
   bool  res = false;
   //Получаем список каталогов
   QStringList lstDirs  = dir.entryList(QDir::Dirs  |
                                   QDir::AllDirs |
                                   QDir::NoDotAndDotDot);
   //Получаем список файлов   
   QStringList lstFiles = dir.entryList(QDir::Files);
   //Удаляем файлы
   foreach (QString entry, lstFiles)
   {
      QString entryAbsPath = dir.absolutePath() + "/" + entry;
      QFile::remove(entryAbsPath);
   }
   //Для папок делаем рекурсивный вызов    
   foreach (QString entry, lstDirs)
   {
      QString entryAbsPath = dir.absolutePath() + "/" + entry;
      removeFolder(QDir(entryAbsPath));
   }
   //Удаляем обрабатываемую папку
   if (!QDir().rmdir(dir.absolutePath()))
   {
      res = true;
   }
   return res;
}

void PsoExplorerClass::deleteCurrentData()
{
	FileExplorerClass feMarks;
}