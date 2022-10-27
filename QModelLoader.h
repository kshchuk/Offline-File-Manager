#pragma once
#include <QFileSystemModel>
#include <QString>

#include "QJsonModel.h"


class QModelLoader
{
	QAbstractItemModel* model;

public:
	QModelLoader(QAbstractItemModel* model)
	{
		this->model = model;
	};
	//~QModelLoader();

	void readModel(QString fileName, QList<QString> headers);
	void writeModel(QString fileName);

	void makeFileSystemModel(QString root = "", int maxLoadedDepth = 10);

	QAbstractItemModel* getModel();

private:
	std::string convertRecursively(QModelIndex parent = QModelIndex());
	std::string toJson(const std::string& str); // converts to json style
	void loadRecursively(int currentDepth = 0, int maxDepth = 5, 
		QModelIndex parent = QModelIndex());
};

