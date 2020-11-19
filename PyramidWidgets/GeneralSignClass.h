
#include "ui_GenSigns.h"
#include <QString>
#include <QDialog>

class FileExplorerClass;

class SignClass : public QDialog, public Ui::OmenMenu
{
	Q_OBJECT
public:
	SignClass(QWidget *parent);

private:
	void _initSigns();
	bool _loadSettings();
private:
	QString _fullPathToSigns;
	QString _pathPhotosSigns;
	FileExplorerClass* _signsPhotoExp;
private slots:
	void _slotBack();
	void _slotNextPhoto();
	void _slotPervPhoto();
	void _slotUpText();
	void _slotDownText();
	void _slotButtonClickSound();
};