
#include <QtGui>
#include <QFileInfoList>

class FileExplorerClass : public QObject
{
	Q_OBJECT
public:
	FileExplorerClass(QObject* parent = 0,
								QString dirPath = tr("null") /*���� � ������*/, 
								QStringList fileExt = QStringList() << "*.jpg" /* ���������� ������*/ );
	~FileExplorerClass();
	//�������� ���� � ������ ������ �����
	QString getLastFile(bool fullPath = true);
	//�������� ���� � ���������� �����
	QString getNextFile(bool fullPath = true);
	//�������� ���� � ����������� �����
	QString getPervFile(bool fullPath = true);
	//�������� ������� ����
	QString getCurrentFile(bool fullPath = true);
	//���������� ����� ���� ��� ������ �� ���������
	bool setDefPathOfFile(QString path);
	//������� ������� ����
	bool deleteCurrentFile();
	bool deleteAllFiles();
	bool backUpAllFiles(short type = 0,QString psoName = "NULL"); //0 ����� // 1 ���� // 2 ���� // 3 �������� // 4 ������� // 5 �����
	//���������� ����� ���������� ��� ������
	bool setFileExtension(QStringList extencion);
	//�������� ���������� ������
	short getCountOfFiles();
	//������� ����� �������� �������� (����������� 1)
	short getCurrentIndex();

	void setExtensions(QStringList extensions);

	QStringList getStringListOfFiles();
	static bool copyAllFilesFromDir(QString pathToDir,QString newPathToDir);

private:
	short _index;
	//���� � ������
	QString _pathToFiles;
	//���������� �����
	QStringList _fileExtension;

private:
	//�������� ������ ����� �� ���� ������
	QFileInfoList getListOfFiles();
	//��������� ������
	bool _incIndex();
	//��������� ������
	bool _decIndex();
};
