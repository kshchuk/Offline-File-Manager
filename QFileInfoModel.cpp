#include "QFileInfoModel.h"

#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QApplication>
#include <QMimeType>
#include <QLocale>
#include <QStyle>

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

	QStandardItem* root = this->invisibleRootItem();

	foreach(auto drive, QDir::drives())
	{
		QDirIterator it(drive.absoluteFilePath());

		QList<QStandardItem*> preparedRow = packDrive(it);
		root->appendRow(preparedRow);

		int row = root->rowCount() - 1;
		QModelIndex index = root->child(row)->index();

		readHierarchyRecursive(index, it.path(), maxDepth);
	}
	initFileModelHeaders(this);
	return this;
}

QAbstractItemModel* QFileInfoModel::genExternalDrivesModel(size_t maxDepth)
{
	this->clear();

	QStandardItem* root = this->invisibleRootItem();

	QList<QString> externalDrives = removableDrives();
	foreach(auto drive, externalDrives)
	{
		QDirIterator it(drive);

		QList<QStandardItem*> preparedRow = packDrive(it);
		root->appendRow(preparedRow);

		int row = root->rowCount() - 1;
		QModelIndex index = root->child(row)->index();

		readHierarchyRecursive(index, it.path(), maxDepth);
	}
	initFileModelHeaders(this);
	return this;
}

void QFileInfoModel::readHierarchyRecursive(QModelIndex parent, const QString& path, size_t maxDepth, size_t curDepth)
{
	if (curDepth > maxDepth)
		return;

	QStandardItem* parentItem = this->itemFromIndex(parent);

	QDirIterator it(path, QDir::NoDotAndDotDot | QDir::AllEntries);
	while (it.hasNext())
	{
		it.next();

		QList<QStandardItem*> preparedRow = fromFileInfo(it.fileInfo());
		parentItem->appendRow(preparedRow);

		int row = parentItem->rowCount() - 1;
		QModelIndex index = parentItem->child(row)->index();


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

QString QFileInfoModel::fileSize(const QFileInfo& info) const
{
	if (info.isFile())
	{
		quint64 size = info.size();
		QLocale locale;
		return locale.formattedDataSize(size);
	}
	return QString();
}

QList<QStandardItem*> QFileInfoModel::packDrive(const QDirIterator& drive) const
{
	// Is drive?
	Q_ASSERT(([&]() -> bool {
		foreach(auto curDrive, QDir::drives())
		{
			QString p1 = curDrive.absolutePath(), p2 = drive.path();
			if (p1 == p2) return true;
		}
		QStringList d = removableDrives();
		foreach(auto curDrive, d)
		{
			curDrive[curDrive.size() - 1] = '/';
			QString p1 = curDrive, p2 = drive.path();
			if (p1 == p2) return true;
		}
		return false;
		})());

	QList<QStandardItem*> row;
	
	row.append(new QStandardItem(this->iconProvider.icon(drive.fileInfo()), drive.path()));
	row.append(new QStandardItem(fileSize(drive.fileInfo())));

	QMimeType mime = db.mimeTypeForFile(drive.path());
	row.append(new QStandardItem(mime.comment()));
	row.append(new QStandardItem(drive.fileInfo().lastModified().toString()));

	return row;
}

QList<QStandardItem*> QFileInfoModel::fromFileInfo(const QFileInfo& info) const
{
	QList<QStandardItem*> row;

	row.append(new QStandardItem(this->iconProvider.icon(info), info.fileName()));
	row.append(new QStandardItem(fileSize(info)));

	QMimeType mime = db.mimeTypeForFile(info);
	row.append(new QStandardItem(mime.comment()));
	row.append(new QStandardItem(info.lastModified().toString()));

	return row;
}
