
#include <QtGui>
#include <QFileInfoList>

class FileExplorerClass : public QObject
{
	Q_OBJECT
public:
	FileExplorerClass(QObject* parent = 0,
								QString dirPath = tr("null") /*путь к файлам*/, 
								QStringList fileExt = QStringList() << "*.jpg" /* расширение файлов*/ );
	~FileExplorerClass();
	//получить путь к самому новому файлу
	QString getLastFile(bool fullPath = true);
	//получить путь к следующему файлу
	QString getNextFile(bool fullPath = true);
	//получить путь к предыдущему файлу
	QString getPervFile(bool fullPath = true);
	//получить текущий файл
	QString getCurrentFile(bool fullPath = true);
	//установить новый путь для файлов по умолчанию
	bool setDefPathOfFile(QString path);
	//удалить текущий файл
	bool deleteCurrentFile();
	bool deleteAllFiles();
	bool backUpAllFiles(short type = 0,QString psoName = "NULL"); //0 любые // 1 трек // 2 фото // 3 маршруты // 4 палетки // 5 видео
	//установить новое расшерение для файлов
	bool setFileExtension(QStringList extencion);
	//получить количество файлов
	short getCountOfFiles();
	//вернуть номер текущего элемента (минимальный 1)
	short getCurrentIndex();

	void setExtensions(QStringList extensions);

	QStringList getStringListOfFiles();
	static bool copyAllFilesFromDir(QString pathToDir,QString newPathToDir);

private:
	short _index;
	//путь к файлам
	QString _pathToFiles;
	//расширение файла
	QStringList _fileExtension;

private:
	//получить список путей ко всем файлам
	QFileInfoList getListOfFiles();
	//увеличить индекс
	bool _incIndex();
	//уменьшить индекс
	bool _decIndex();
};
