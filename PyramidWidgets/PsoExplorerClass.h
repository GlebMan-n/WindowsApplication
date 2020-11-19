#include <QtGui>
#include <QFileInfoList>

//Получаем имя последнего используемого ПСО из файла INI
//восстанавливаем его если пользователь выбрал продолжить ПСО
//если пользователь решил работать с новым ПСО
//предупреждаем что данные будут удалены и начинаем новое ПСО
//имя нового ПСО текущие дата и время 
//данные ПСО хранятся в папке tmp/PSO_old_data/датавремя создания псо/соответствующие типу данных папки

class PsoExplorerClass : public QObject
{
	Q_OBJECT
public:
	PsoExplorerClass(QObject *parent = 0);
	~PsoExplorerClass();
	//дата текущего псо
	QDate getDateOfCurrentPso();
	//время текущего псо
	QTime getTimeOfCurrentPso();
	//имя текущего псо ДД.ММ.ГГГГ ЧЧ.ММ
	QString getNameOfCurrentPso();
	//тест
	bool backupPso();
	bool createPso();
	void deleteCurrentData();

private:
	//путь к папке ПСО
	QString _pathToPso;
	//Список сохраненных ПСО
	QStringList _listOfPSO;
	//Текущее не оконченное ПСО
	//Имя ПСО дата время в папке в формате ДДММГГГГЧЧММ
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
	//загрузка имени текущего ПСО из файла настроек
	bool _loadSettings();
	//Сохранение имени текущего ПСО в файл настроек
	bool _saveSettings();
	//удалить текущее ПСО
	bool _delPso(QString nameOfPso);
	//восстановить текущее псо
	bool _restorePso(QString nameOfPso);

	//получить список сохраненных псо
	QStringList _getListOfPso(QString pathToPso);
	//создать новое псо
	QString _createNewPso(QString pathToPso);
	//копируем файлы в папку соответствующего псо 
	//(все папки с текущими данными)
	bool _copyCurrentPso();
	//Удаляем данные ткущего псо из папки DATA
	bool _erasePsoData();
	bool removeFolder(QDir & dir);
private slots:
	//слоты для внешнего доступа к функциям
	void _slotDelPso(QString nameOfPso);
	void _slotRestorePso(QString nameOfPso);
	void _slotNewPso();


signals:
	//отправить список ПСО
	void signListOfPso(QStringList listOfPso);
	//отправить текущее ПСО
	void signCurrentPso(QString currentPsoName);
};
