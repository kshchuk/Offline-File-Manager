#pragma once

#include <QObject>
#include <QTest>

#include "QFileInfoModel.h"

class QFileInfoModel_test  : public QObject
{
	Q_OBJECT


private slots:
	void genFromGoogleDriveResponse();

	void genExternalDrivesModel();
	void genStaticSystemModel();
	void genGoogleDriveModel();

	void insertFileLinkToTheFolder();
	void insertFileToTheFolder();
	void appendFolder();
	void deleteFile();
	void setName();

	void getPath();
	void getPathfFromInfo();
	void byPath();
	void pathFromStringList();

	void readFile();
	void writeFile();

private:
	//bool compareRecursive(QStandardItem* item1, QStandardItem* item2, 
									//size_t maxDepth,size_t curDepth = 0);
};
