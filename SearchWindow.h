#pragma once

#include <QDialog>
#include <QFileIconProvider>
#include "ui_SearchWindow.h"

#include "QFileInfoModel.h"
#include "Searcher.h"

class SearchWindow : public QDialog
{
	Q_OBJECT

public:
	SearchWindow(QFileInfoModel* model, QString path = "", QWidget* parent = nullptr);
	~SearchWindow();

signals:
	void infoSent(QModelIndex);

private slots:
	void search();
	void foundSlot(QModelIndex);
	void open();
	void on_customContextMenu(const QPoint& point);

private:
	Ui::SearchWindowClass ui;
	QFileInfoModel* model;
	Searcher* searcher;
	QFileIconProvider provider;
	QString path;

	QModelIndexList foundIndexes;
	void initTable();
};
