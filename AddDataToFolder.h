#pragma once

#include <QDialog>
#include <QFileIconProvider>
#include "QFileInfoModel.h"
#include "ui_AddDataToFolder.h"

class AddDataToFolder : public QDialog
{
	Q_OBJECT

public:
	AddDataToFolder(QWidget *parent = nullptr, QFileInfoModel* = nullptr);
	~AddDataToFolder();

signals:
	void infoSent(QModelIndexList, QStringList);
	void finished();


private slots:
	void on_treeView_clicked(const QModelIndex& index);
	void selectFromExplorer();
	void sendInfo();
	void clearAll();
	void removeFromList();
	void on_customContextMenu(const QPoint& point);

private:
	Ui::AddDataToFolderClass ui;
	QFileInfoModel* model;

	QList<QPair<QModelIndex, QString>> selected;
	QStringList selectedExternal;
	QFileIconProvider icons;

	void treeViewInit(QTreeView* tree, QAbstractItemModel* model);
	void updateSelectedFromExplorer();
};
