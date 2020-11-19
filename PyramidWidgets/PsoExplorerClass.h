#include <QtGui>
#include <QFileInfoList>

//�������� ��� ���������� ������������� ��� �� ����� INI
//��������������� ��� ���� ������������ ������ ���������� ���
//���� ������������ ����� �������� � ����� ���
//������������� ��� ������ ����� ������� � �������� ����� ���
//��� ������ ��� ������� ���� � ����� 
//������ ��� �������� � ����� tmp/PSO_old_data/��������� �������� ���/��������������� ���� ������ �����

class PsoExplorerClass : public QObject
{
	Q_OBJECT
public:
	PsoExplorerClass(QObject *parent = 0);
	~PsoExplorerClass();
	//���� �������� ���
	QDate getDateOfCurrentPso();
	//����� �������� ���
	QTime getTimeOfCurrentPso();
	//��� �������� ��� ��.��.���� ��.��
	QString getNameOfCurrentPso();
	//����
	bool backupPso();
	bool createPso();
	void deleteCurrentData();

private:
	//���� � ����� ���
	QString _pathToPso;
	//������ ����������� ���
	QStringList _listOfPSO;
	//������� �� ���������� ���
	//��� ��� ���� ����� � ����� � ������� ������������
	QString _currentPsoName;
	
	QString pathToMarks;
	QString pathToMRoute;
	QString pathToVideo;
	QString pathToPhoto;
	QString pathToTracks;
	QString pathToRoute;
	QString pathToGrid;
	short _numbOfSelectedPso;

private:
	//�������� ����� �������� ��� �� ����� ��������
	bool _loadSettings();
	//���������� ����� �������� ��� � ���� ��������
	bool _saveSettings();
	//������� ������� ���
	bool _delPso(QString nameOfPso);
	//������������ ������� ���
	bool _restorePso(QString nameOfPso);

	//�������� ������ ����������� ���
	QStringList _getListOfPso(QString pathToPso);
	//������� ����� ���
	QString _createNewPso(QString pathToPso);
	//�������� ����� � ����� ���������������� ��� 
	//(��� ����� � �������� �������)
	bool _copyCurrentPso();
	//������� ������ ������� ��� �� ����� DATA
	bool _erasePsoData();
	bool removeFolder(QDir & dir);
private slots:
	//����� ��� �������� ������� � ��������
	void _slotDelPso(QString nameOfPso);
	void _slotRestorePso(QString nameOfPso);
	void _slotNewPso();


signals:
	//��������� ������ ���
	void signListOfPso(QStringList listOfPso);
	//��������� ������� ���
	void signCurrentPso(QString currentPsoName);
};
