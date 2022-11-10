#pragma once

#include <QMainWindow>
#include <QProcess>

#include "ui_Robocopy.h"

#include "QFileInfoModel.h"


class Robocopy : public QMainWindow
{
	Q_OBJECT

public:
	Robocopy(QWidget *parent, QFileInfoModel* model);
	~Robocopy();

private slots:
	void seeShortDescription();
	void visitReferenceSite();
	void updatePsevdoConsole();
	void setSource();
	void setSourceSlot (QString);
	void setDest();
	void setDestSlot(QString);
	void runCommand();
	void updateRunning();
	void processFinished(int code);


private:
	Ui::RobocopyClass ui;
	QList<QPair<QCheckBox*, QString>> checkBoxes;

	QFileInfoModel* model;
	QProcess* process;
	QString prevCommand;
};
