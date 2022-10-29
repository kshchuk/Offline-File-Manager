//#include "QModelLoader.h"
//
//#include <stdexcept>
//
//#include <QFileSystemModel>
//#include <QDirIterator>
//#include <QMimeDatabase>
//#include <QMimeType>
//
//#include "ModelSerializer.h"
//#include "system_depend_functions.h"
//
//
//QAbstractItemModel* QModelLoader::readModel(QString fileName)
//{
//	QFile file(fileName);
//	if (file.open(QIODevice::ReadOnly)) {
//		QDataStream stream(&file);
//		if (model) delete model;
//		model = new QStandardItemModel();
//
//		ModelSerializer<> ser;
//		ser.load(stream, model);
//		file.close();
//		return model;
//	}
//	else
//		throw std::runtime_error("Unable to open file" + fileName.toStdString());
//
//	return nullptr;
//}
//
//void QModelLoader::writeModel(QString fileName, size_t maxDepth) const
//{
//	QFile file(fileName);
//	if (file.open(QIODevice::WriteOnly)) {
//		QDataStream stream(&file);
//		
//		ModelSerializer<> ser;
//		ser.save(stream, model);
//
//		file.flush();
//		file.close();
//	} 
//	else
//		throw std::runtime_error("Unable to open file" + fileName.toStdString());
//}
//
//QAbstractItemModel* QModelLoader::genExternalDrivesModel(size_t maxDepth)
//{
//	if (!model) delete model;
//	QStandardItemModel* smodel = new QStandardItemModel; model = smodel;
//
//	QModelIndex root = smodel->invisibleRootItem()->index();
//
//	smodel->insertColumn(0, root);
//
//	QList<QString> externalDrives = removableDrives();
//	foreach(auto drive, externalDrives)
//	{
//		QDirIterator it(drive);
//
//		int row = smodel->rowCount(root);
//		smodel->insertRow(row, root);
//
//		QModelIndex index = smodel->index(row, 0, root);
//
//		model->setData(index, it.path());
//
//		// smodel->setItemData(index, fromFileInfo(it.fileInfo()));
//		
//		readHierarchyRecursive(index, it.path(), maxDepth);
//	}
//	initFileModel(smodel);
//
//	return model;
//}
//
//QAbstractItemModel* QModelLoader::genStaticSystemModel(size_t maxDepth)
//{
//	if (!model) delete model;
//
//	QStandardItemModel* smodel = new QStandardItemModel; model = smodel;
//	initFileModel(smodel);
//	QModelIndex root = smodel->invisibleRootItem()->index();
//
//
//	smodel->insertColumn(0, root);
//
//	foreach(auto drive, QDir::drives())
//	{
//		QDirIterator it(drive.absolutePath());
//
//		int row = model->rowCount(root);
//		model->insertRow(row, root);
//
//		QModelIndex index = model->index(row, 0, root);
//		smodel->setData(index, it.fileName());
//		// smodel->setItemData(index, fromFileInfo(it.fileInfo()));
//
//		readHierarchyRecursive(index, it.path(), maxDepth);
//	}
//	return model;
//}
//
//QAbstractItemModel* QModelLoader::getModel() const
//{
//	return model;
//}
//
//void QModelLoader::initFileModel(QStandardItemModel* model)
//{
//	QFileSystemModel temp;
//	for (size_t i = 0; i < temp.columnCount(); i++)
//	{
//		QStandardItem* header = new QStandardItem(
//			temp.headerData(i, Qt::Horizontal).toString());
//
//		model->setHorizontalHeaderItem(i, header);
//	}
//}
//
//void QModelLoader::readHierarchyRecursive(QModelIndex parent, const QString& path,
//	size_t maxDepth, size_t curDepth)
//{
//	if (curDepth > maxDepth)
//		return;
//
//	QStandardItemModel* smodel = dynamic_cast<QStandardItemModel*> (model);
//	Q_ASSERT(smodel != nullptr);
//
//	smodel->insertColumn(0, parent);
//
//	QDirIterator it(path, QDir::NoDotAndDotDot | QDir::AllEntries);
//	while (it.hasNext())
//	{
//		it.next();
//
//		int row = model->rowCount(parent);
//		smodel->insertRow(row, parent);
//
//		QString str = it.fileName();
//
//		QModelIndex index = model->index(row, 0, parent);
//		// model->setData(index, it.fileName());
//		QStandardItem* item = new QStandardItem(1, 4);
//		// smodel->setItemData(index, fromFileInfo(it.f
//
//		if (it.fileInfo().isDir())
//			readHierarchyRecursive(index, it.filePath(), maxDepth, curDepth + 1);
//	}
//}
//
//QVariantMap QModelLoader::fromFileInfo(const QFileInfo& info)
//{
//	QVariantMap mapvar;
//
//	mapvar.insert("Name", info.fileName());
//	mapvar.insert("Size", info.size());
//
//	QMimeDatabase db; QMimeType mime = db.mimeTypeForFile(info);
//	mapvar.insert("Type", mime.name());
//	mapvar.insert("Date Modified", info.lastModified());
//
//	return mapvar;
//}
//
