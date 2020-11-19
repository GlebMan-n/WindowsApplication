
#include <PyramidWidgets\FileExplorerClass.h>

FileExplorerClass::FileExplorerClass(QObject* parent,
								QString dirPath /*путь к файлам*/, 
								QStringList fileExt /* расширение файлов*/)
{
	this->setParent(parent);
	_pathToFiles = dirPath;
	_fileExtension = fileExt;
	_index = 0;
}
FileExplorerClass::~FileExplorerClass()
{
}

//получить путь к самому новому файлу
QString FileExplorerClass::getLastFile(bool fullPath)
{
	if(getCountOfFiles() < 1)
		return "null";
	QString lastFile;
	if(fullPath)
		lastFile = getListOfFiles().at(0).absoluteFilePath();
	else 
		lastFile = getListOfFiles().at(0).fileName();

	return lastFile;
}

//получить путь к следующему файлу
QString FileExplorerClass::getNextFile(bool fullPath)
{
	if(getCountOfFiles() < 1)
		return "null";

	if (!_incIndex())
		return getCurrentFile(fullPath);

	QString nextFile;

	if(fullPath)
		nextFile = getListOfFiles().at(_index).absoluteFilePath();
	else 
		nextFile = getListOfFiles().at(_index).fileName();

	return nextFile;
}

//получить путь к предыдущему файлу
QString FileExplorerClass::getPervFile(bool fullPath)
{
	if(getCountOfFiles() < 1)
		return "null";

	if (!_decIndex())
	return getCurrentFile(fullPath);

	QString pervFile;

	if(fullPath)
		pervFile = getListOfFiles().at(_index).absoluteFilePath();
	else 
		pervFile = getListOfFiles().at(_index).fileName();

	return pervFile;
}
//получить текущий файл
QString FileExplorerClass::getCurrentFile(bool fullPath)
{
	if(getCountOfFiles() < 1)
		return "null";

	QString currentFile;
	if(fullPath)
		currentFile = getListOfFiles().at(_index).absoluteFilePath();
	else 
		currentFile = getListOfFiles().at(_index).fileName();

	return currentFile;
}

//установить новый путь для файлов по умолчанию
bool FileExplorerClass::setDefPathOfFile(QString path)
{
	if(path.isNull())
		return false;
	_pathToFiles = path;
	return true;
}

//удалить текущий файл
bool FileExplorerClass::deleteCurrentFile()
{
	short numbOfFiles = getCountOfFiles();
	if (numbOfFiles < 1) 
		return false;
	//удаляем трек под текущим индексом
	QString path(getListOfFiles().at(_index).absoluteFilePath());

	QFile track(path);
	if(track.open(QIODevice::ReadWrite))
	{
		track.remove();
		track.close();
		_decIndex();
		return true;
	}
	return false;
}

//установить новое расшерение для файлов
bool FileExplorerClass::setFileExtension(QStringList extencion)
{
	_fileExtension = extencion;
	return true;
}


//получить количество файлов
short FileExplorerClass::getCountOfFiles()
{
	short numbOfFiles = 0;
	numbOfFiles = getListOfFiles().count();
	return numbOfFiles;
}


//получить список путей ко всем файлам
QFileInfoList FileExplorerClass::getListOfFiles()
{
	QFileInfoList fileList;
	//заполняем список треков
	QDir dir(_pathToFiles);
  //  fileList = dir.entryInfoList(QStringList() << "*." +_fileExtension, QDir:: );
	fileList = dir.entryInfoList(QStringList() << _fileExtension, QDir::Files, QDir::Time);
	return fileList;
}

//увеличить индекс
bool FileExplorerClass::_incIndex()
{
	short numbOfFiles = getCountOfFiles();
	if(numbOfFiles<1)
		return false;

	if (_index == numbOfFiles-1)
	{
		//не изменяем _index, если текущий индекс крайний
		return false;
	}
	else
	{
		//увеличиваем _index если текущий индекс не крайний
		_index++;
		return true;
	}
}

//уменьшить индекс
bool FileExplorerClass::_decIndex()
{
	if(getCountOfFiles() < 1)
		return false;
	if (_index == 0)
	{
		//не изменяем _index, если текущий индекс крайний
		return false;
	}
	else
	{
		//уменьшаем _index если текущий индекс не крайний
		_index--;
		return true;
	}
}

short FileExplorerClass::getCurrentIndex()
{
	return _index+1;
}

QStringList FileExplorerClass::getStringListOfFiles()
{
	QStringList result;
	for (int i = 0; i< getListOfFiles().size(); i++)
	{
		result << getListOfFiles().at(i).absoluteFilePath ();
	}
	return result;
}

bool FileExplorerClass::deleteAllFiles()
{
	short numbOfFiles = getCountOfFiles();
	if (numbOfFiles < 1) 
		return false;
	QFileInfoList infoList(getListOfFiles());
	for (short i = 0; i < infoList.size(); i++)
	{
		QString path(infoList.at(i).absoluteFilePath());
		QFile file(path);
		if(file.exists())
		{
			file.remove();
		}
	}
	
	return true;
}

bool FileExplorerClass::backUpAllFiles(short type,QString psoName)
{
	short numbOfFiles = getCountOfFiles();
	if (numbOfFiles < 1) 
		return false;
	QFileInfoList infoList(getListOfFiles());

	for (short i = 0; i < infoList.size(); i++)
	{
		QString path(infoList.at(i).absoluteFilePath());
		QFile file(path);
		if(file.exists())
		{
			QString folder("\\");
			QDir dir;
			switch (type)
			{
			case 0:
				break;
			case 1:
				folder = "track\\";
				dir.mkpath(QCoreApplication::applicationDirPath ()+"\\tmp\\"+ psoName + "\\"+folder);
				break;
			case 2:
				folder = "photo\\";
				dir.mkpath(QCoreApplication::applicationDirPath ()+"\\tmp\\"+ psoName + "\\"+folder);
				break;
			case 3:
				folder = "route\\";
				dir.mkpath(QCoreApplication::applicationDirPath ()+"\\tmp\\"+ psoName + "\\"+folder);
				break;
			case 4:
				folder = "grid\\";
				dir.mkpath(QCoreApplication::applicationDirPath ()+"\\tmp\\"+ psoName + "\\"+folder);
				break;
			case 5:
				folder = "video\\";
				dir.mkpath(QCoreApplication::applicationDirPath ()+"\\tmp\\"+ psoName + "\\"+folder);
				break;
			}
			QString tmp(QCoreApplication::applicationDirPath ()+"\\tmp\\" + psoName + "\\" +folder+ getListOfFiles().at(i).fileName());
			file.copy(tmp);
		}
	}
	return true;
}

bool FileExplorerClass::copyAllFilesFromDir(QString pathToDir, QString newPathToDir)
{
	QFileInfoList fileList;
	QDir dir(pathToDir);
	//есть ли директория
	if(dir.exists())
	{
		QDir dirDest(newPathToDir);
		//есть ли директория куда копировать файлы
		if(!dirDest.exists())
			dirDest.mkdir(newPathToDir);


		fileList = dir.entryInfoList(QStringList() << "*.*", QDir::Files, QDir::Time);
		
		for (short i = 0; i < fileList.size(); i++)
		{
			QFile file(fileList.at(i).absoluteFilePath());
			QString tmp(newPathToDir +"\\"+ fileList.at(i).fileName());
			file.copy(tmp);
		}

		return true;
	}
	else
		return false;
}