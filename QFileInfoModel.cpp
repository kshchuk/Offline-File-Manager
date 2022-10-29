#include "QFileInfoModel.h"

#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QMimeType>

#include "ModelSerializer.h"
#include "system_depend_functions.h"


QFileInfoModel::QFileInfoModel(QObject *parent)
	: QStandardItemModel(parent)
{
	QFileSystemModel temp;
	for (size_t i = 0; i < temp.columnCount(); i++)
	{
		QStandardItem* header = new QStandardItem(
			temp.headerData(i, Qt::Horizontal).toString());

		this->setHorizontalHeaderItem(i, header);
	}
}

QFileInfoModel::~QFileInfoModel()
{}

QAbstractItemModel* QFileInfoModel::readFile(QString fileName)
{
	this->clear();

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);
		this->clear();

		ModelSerializer<> ser;
		ser.load(stream, this);
		file.close();
		return this;
	}
	else
		throw std::runtime_error("Unable to open file" + fileName.toStdString());

	return nullptr;
}

void QFileInfoModel::writeFile(QString fileName, size_t maxDepth) const
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);

		ModelSerializer<> ser;
		ser.save(stream, this);

		file.flush();
		file.close();
	}
	else
		throw std::runtime_error("Unable to open file" + fileName.toStdString());
}

QAbstractItemModel* QFileInfoModel::genStaticSystemModel(size_t maxDepth)
{
	this->clear();

	QModelIndex root = this->invisibleRootItem()->index();
	this->insertColumn(0, root);

	foreach(auto drive, QDir::drives())
	{
		QDirIterator it(drive.absolutePath());

		int row = this->rowCount(root);
		this->insertRow(row, root);

		QModelIndex index = this->index(row, 0, root);
		this->setData(index, it.fileName());
		// smodel->setItemData(index, fromFileInfo(it.fileInfo()));

		readHierarchyRecursive(index, it.path(), maxDepth);
	}
	return this;
}

QAbstractItemModel* QFileInfoModel::genExternalDrivesModel(size_t maxDepth)
{
	this->clear();

	QModelIndex root = this->invisibleRootItem()->index();

	this->insertColumn(0, root);

	QList<QString> externalDrives = removableDrives();
	foreach(auto drive, externalDrives)
	{
		QDirIterator it(drive);

		int row = this->rowCount(root);
		this->insertRow(row, root);

		QModelIndex index = this->index(row, 0, root);

		this->setData(index, it.path());

		// smodel->setItemData(index, fromFileInfo(it.fileInfo()));

		readHierarchyRecursive(index, it.path(), maxDepth);
	}
	initFileModelHeaders(this);
	return this;
}

void QFileInfoModel::readHierarchyRecursive(QModelIndex parent, const QString& path, size_t maxDepth, size_t curDepth)
{
	if (curDepth > maxDepth)
		return;

	this->insertColumn(0, parent);

	QDirIterator it(path, QDir::NoDotAndDotDot | QDir::AllEntries);
	while (it.hasNext())
	{
		it.next();

		int row = this->rowCount(parent);
		this->insertRow(row, parent);

		QString str = it.fileName();

		QModelIndex index = this->index(row, 0, parent);
		// model->setData(index, it.fileName());
		// QStandardItem* item = new QStandardItem(1, 4);
		this->setData(index, it.fileInfo().fileName());

		if (it.fileInfo().isDir())
			readHierarchyRecursive(index, it.filePath(), maxDepth, curDepth + 1);
	}
}

void QFileInfoModel::initFileModelHeaders(QFileInfoModel* model) const
{
	QFileSystemModel temp;
	for (size_t i = 0; i < temp.columnCount(); i++)
	{
		QStandardItem* header = new QStandardItem(
			temp.headerData(i, Qt::Horizontal).toString());

		model->setHorizontalHeaderItem(i, header);
	}
}

QVariantMap QFileInfoModel::fromFileInfo(const QFileInfo& info) const
{
	QVariantMap mapvar;

	mapvar.insert("Name", info.fileName());
	mapvar.insert("Size", info.size());

	QMimeDatabase db; QMimeType mime = db.mimeTypeForFile(info);
	mapvar.insert("Type", mime.name());
	mapvar.insert("Date Modified", info.lastModified());

	return mapvar;
}
