#include "ui_DialogUrl.h"
#include <QtGui>

class DialogInputUrl : public QDialog, public Ui::InputUrl
{
	Q_OBJECT
public:
	DialogInputUrl(QWidget* parent = 0, QStringList listOfAdr = QStringList());
	~DialogInputUrl();
	QStringList result;
private:
	void _setAdress(QStringList listOfAdr);
private slots:
	void _slotApply();
	void _slotCancel();
	void _slotKeyboard();
};