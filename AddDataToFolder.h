#pragma once

#include <QDialog>
#include "QFileInfoModel.h"
#include "ui_AddDataToFolder.h"

class AddDataToFolder : public QDialog
{
	Q_OBJECT

public:
	AddDataToFolder(QWidget *parent = nullptr, QFileInfoModel* = nullptr);
	~AddDataToFolder();

signals:
	void indexesSent(QModelIndexList);
	void finished();


private slots:
	void on_treeView_clicked(const QModelIndex& index);
	//void addressText_edited();
	void sendIndexes();
	//void enterPressEvent(QKeyEvent* event);

private:
	Ui::AddDataToFolderClass ui;
	QFileInfoModel* model;

	QList<QPair<QModelIndex, QString>> selected;
	void treeViewInit(QTreeView* tree, QAbstractItemModel* model);
	//void updateAddressText();
};
