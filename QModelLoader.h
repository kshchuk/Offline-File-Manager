//#pragma once
//
//#include <QStandardItemModel>
//#include <QFileInfo>
//#include <QString>
//#include <QFile>
//
//// #include "QJsonModel.h"
//
//
//class QModelLoader
//{
//public:
//	QModelLoader() {};
//	QModelLoader(QAbstractItemModel* model) : model(model) {};
//
//	QAbstractItemModel* getModel() const;
//
//	QAbstractItemModel* readModel(QString fileName);
//	void writeModel(QString fileName, size_t maxDepth = 20) const;
//
//	QAbstractItemModel* genExternalDrivesModel(size_t maxDepth = 20);
//	QAbstractItemModel* genStaticSystemModel(size_t maxDepth = 20);
//
//
//private:
//	QAbstractItemModel* model = nullptr;
//
//	static void initFileModel(QStandardItemModel* model);
//	void readHierarchyRecursive(QModelIndex parent, const QString& path,
//		size_t maxDepth = 20, size_t curDepth = 1);
//	QVariantMap fromFileInfo(const QFileInfo& info);
//};
//
