#include "QFileInfoModel.h"

#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QException>
#include <QApplication>
#include <QMimeType>
#include <QLocale>
#include <QStyle>

#include <functional>

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

QList<QString> QFileInfoModel::getPath(QModelIndex index) const
{
	QList<QString> path;
	while (index.isValid())
	{
		path.append(index.data().toString());
		index = index.parent();
	}
	std::reverse(path.begin(), path.end());

	return path;
}

QAbstractItemModel* QFileInfoModel::readFile(QString fileName)
{
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);
		this->clear();
		ModelSerializer<> ser;
		Status st = ser.load(stream, this);
		if (!st.ok()) throw std::runtime_error("Reading error:" + fileName.toStdString());
		this->initFileModelHeaders(this);

		setIcons();

		file.close();
		return this;
	}
	else
		throw std::runtime_error("Unable to open file:" + fileName.toStdString());

	return nullptr;
}

void QFileInfoModel::writeFile(QString fileName, size_t maxDepth) const
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);

		ModelSerializer<> ser;
		Status st = ser.save(stream, this);
		if (!st.ok()) throw std::runtime_error("Writing error:" + fileName.toStdString());

		file.flush();
		file.close();
	}
	else
		throw std::runtime_error("Unable to open file: " + fileName.toStdString());
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

void QFileInfoModel::setIcons(const QModelIndex& index, int depth)
{
	// TODO: Restore icons
	//if (index.isValid())
	//{
	//	QString name = index.data().toString();
	//	QString iconName = index.siblingAtColumn(4).data().toString();

	//	QStandardItem* oldItem = this->itemFromIndex(index);

	// 	QList<QStandardItem*> newItem = packInfo(index);

	//	for (size_t i = 0; i < oldItem->rowCount(); i++)
	//	newItem[0]->setChild(i, oldItem->child(i));

	//	QModelIndex parent = index.parent();
	//	QStandardItem* pItem = this->itemFromIndex(index);
	//	pItem->appendRow(newItem);
	//	pItem->removeRow(oldItem->row());
	//}

	//if ((index.flags() & Qt::ItemNeverHasChildren) || !this->hasChildren(index)) return;
	//
	//int rows = this->rowCount(index);

	//for (int i = 0; i < rows; ++i)
	//	setIcons(this->index(i, 0, index), depth + 1);
}

void QFileInfoModel::readHierarchyRecursive(QModelIndex parent, const QString& path, 
	size_t maxDepth, size_t curDepth)
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

QList<QStandardItem*> QFileInfoModel::packInfo(const QModelIndex& index) const
{
	QList<QStandardItem*> row;

	row.append(new QStandardItem(QIcon::fromTheme(
		this->itemData(index)[(int)ColunmsOrder::ICON_NAME].toString()),
		this->itemData(index)[(int)ColunmsOrder::NAME].toString()));
	for (size_t i = 1; i < this->columnCount(index); i++)
		row.append(new QStandardItem(this->itemData(index)[i].toString()));

	return row;
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
	row.reserve(columnsNumber);
	
	row.insert(int(ColunmsOrder::NAME), new QStandardItem(this->iconProvider.icon(drive.fileInfo()), drive.path()));
	row.insert(int(ColunmsOrder::SIZE), new QStandardItem(fileSize(drive.fileInfo())));

	QMimeType mime = db.mimeTypeForFile(drive.path());
	row.insert(int(ColunmsOrder::TYPE), new QStandardItem(mime.comment()));
	row.insert(int(ColunmsOrder::DATE_MODIDFIED), new QStandardItem(drive.fileInfo().lastModified().toString()));

	row.insert(int(ColunmsOrder::ICON_NAME), new QStandardItem(mime.iconName()));
	row.insert(int(ColunmsOrder::DATE_CREATED), new QStandardItem(drive.fileInfo().birthTime().toString()));
	row.insert(int(ColunmsOrder::GROUP), new QStandardItem(drive.fileInfo().group()));
	row.insert(int(ColunmsOrder::OWNER), new QStandardItem(drive.fileInfo().owner()));
	row.insert(int(ColunmsOrder::OWNER_ID), new QStandardItem(QString::number(drive.fileInfo().ownerId())));
	row.insert(int(ColunmsOrder::CUSTOM_METHADATA), new QStandardItem(QString()));

	return row;
}

QList<QStandardItem*> QFileInfoModel::fromFileInfo(const QFileInfo& info) const
{
	QList<QStandardItem*> row;

	// visible data 
	row.reserve(columnsNumber);
	row.insert(int(ColunmsOrder::NAME), new QStandardItem(this->iconProvider.icon(info), info.fileName()));
	row.insert(int(ColunmsOrder::SIZE), new QStandardItem(fileSize(info)));

	QMimeType mime = db.mimeTypeForFile(info);
	row.insert(int(ColunmsOrder::TYPE), new QStandardItem(mime.comment()));
	row.insert(int(ColunmsOrder::DATE_MODIDFIED), new QStandardItem(info.lastModified().toString()));
	
	//unvisible data
	row.insert(int(ColunmsOrder::ICON_NAME), new QStandardItem(mime.iconName()));
	row.insert(int(ColunmsOrder::DATE_CREATED), new QStandardItem(info.birthTime().toString()));
	row.insert(int(ColunmsOrder::GROUP), new QStandardItem(info.group()));
	row.insert(int(ColunmsOrder::OWNER), new QStandardItem(info.owner()));
	row.insert(int(ColunmsOrder::OWNER_ID), new QStandardItem(QString::number(info.ownerId())));
	row.insert(int(ColunmsOrder::CUSTOM_METHADATA), new QStandardItem(QString()));

	return row;
}
