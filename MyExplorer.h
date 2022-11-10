#pragma once

#include <QTreeView>
#include "QFileInfoModel.h"


class MyExplorer : public QTreeView
{
	Q_OBJECT

public:
	MyExplorer(QWidget* parent, QFileInfoModel* model);
	~MyExplorer();


signals:
	void pathSent(QString path);

public slots:
	void sendPathOfTheSelectedFile();

private:
	void treeViewInit(QTreeView* tree, QFileInfoModel* model1);
};
