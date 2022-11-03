#include "QFileInfoModel.h"

#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QException>
#include <QApplication>
#include <QMimeType>
#include <QLocale>
#include <QStyle>

#include <functional>


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

void QFileInfoModel::readFile(QString fileName, size_t maxDepth)
{
	ReadThread* readThread = new ReadThread(this, fileName, maxDepth);
	connect(readThread, &ReadThread::finished, this, &QFileInfoModel::finished);
	connect(readThread, &ReadThread::error, this, &QFileInfoModel::error);
	connect(readThread, &ReadThread::finished, readThread, &QObject::deleteLater);
	readThread->start();
}

void QFileInfoModel::writeFile(QString fileName)
{
	WriteThread* writeThread = new WriteThread(this, fileName);
	connect(writeThread, &WriteThread::finished, this, &QFileInfoModel::finished);
	connect(writeThread, &WriteThread::error, this, &QFileInfoModel::error);
	connect(writeThread, &WriteThread::finished, writeThread, &QObject::deleteLater);
	writeThread->start();
}

void QFileInfoModel::genStaticSystemModel(size_t maxDepth)
{
	SystemModelGenThread* sysThread = new SystemModelGenThread(this, maxDepth);
	connect(sysThread, &SystemModelGenThread::finished, this, &QFileInfoModel::finished);
	connect(sysThread, &SystemModelGenThread::error, this, &QFileInfoModel::error);
	connect(sysThread, &SystemModelGenThread::finished, sysThread, &QObject::deleteLater);
	sysThread->start();
}

void QFileInfoModel::genExternalDrivesModel(size_t maxDepth)
{
	ExternalDrivesGenThread* exThread = new ExternalDrivesGenThread(this, maxDepth);
	connect(exThread, &ExternalDrivesGenThread::finished, this, &QFileInfoModel::finished);
	connect(exThread, &ExternalDrivesGenThread::error, this, &QFileInfoModel::error);
	connect(exThread, &ExternalDrivesGenThread::finished, exThread, &QObject::deleteLater);
	exThread->start();
}

QModelIndex QFileInfoModel::appendFolder(const QFileInfo& info, QModelIndex parent)
{
	QList<QStandardItem*> erow = fromFileInfo(info);
	erow[(int)ColunmsOrder::NAME]->setData(iconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
	erow[(int)ColunmsOrder::ICON_NAME]->setData(iconProvider.icon(QFileIconProvider::Folder).name(), Qt::DisplayRole);
	erow[(int)ColunmsOrder::TYPE]->setData(virtualFolderType, Qt::DisplayRole);

	if (parent.isValid())
	{
		QStandardItem* iparent = this->itemFromIndex(parent);
		iparent->appendRow(erow);
		int row = iparent->rowCount() - 1;
		return iparent->child(row)->index();
	}
	else {
		this->appendRow(erow);
		int row = this->rowCount() - 1;
		return this->index(row, 0);
	}
}

void QFileInfoModel::setName(QString newName, QModelIndex index)
{
	QStandardItem* item = this->itemFromIndex(index);
	item->setData(newName, 0);
}

QModelIndex QFileInfoModel::byPath(QString path) const
{
	QStringList pieces = path.split("/");
	pieces.removeAll(QString(""));

	if (pieces.isEmpty()) return QModelIndex();
	
	int rows = this->rowCount();
	for (size_t i = 0; i < rows; ++i)
	{
		QString drive = this->index(i, 0).data().toString();
		drive.removeIf([](QChar c) { return c == QChar('/') || c == QChar('\\'); });
		if (!QString::compare(*pieces.constBegin(), drive))
			return byPathRecursive(pieces.constBegin() + 1, 
				pieces.constEnd(), this->index(i, 0));
	}
	return QModelIndex();
}

quint64 QFileInfoModel::fileSize(const QModelIndex& index) const
{
	quint64 sum = 0;
	if (index.isValid())
	{
		QStandardItem* item = this->itemFromIndex(index);
		if (item->hasChildren()) {
			int rows = item->rowCount();
			for (size_t i = 0; i < rows; ++i)
				sum += fileSize(item->child(i)->index());
		}
		else {
			sum = index.siblingAtColumn((int)ColunmsOrder::SIZE_BYTES).data().toLongLong();
		}
	}
	else {
		int rows = this->rowCount();
		for (size_t i = 0; i < rows; ++i)
			sum = fileSize(this->index(i, 0));
	}
	return sum;
}

void QFileInfoModel::setFileName(const QString& name)
{
	fileName = name;
}

void QFileInfoModel::setMaxDepth(size_t d)
{
	maxDepth = d;
}

void QFileInfoModel::setIcons(const QModelIndex& index, int depth)
{
	if (index.isValid())
	{
		QString name = index.data().toString();
		// QString iconName = index.siblingAtColumn((int)ColunmsOrder::ICON_NAME).data().toString();
		QStandardItem* item = this->itemFromIndex(index);
		
		QFileInfo info(name);
		if (!index.siblingAtColumn((int)ColunmsOrder::TYPE).data().toString().compare(virtualFolderType))
			item->setData(iconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
		else
			item->setData(iconProvider.icon(info), Qt::DecorationRole);
	}

	if ((index.flags() & Qt::ItemNeverHasChildren) || !this->hasChildren(index));

	int rows = this->rowCount(index);

	// Folders
	if (this->hasChildren(index) && index.parent().isValid()) {
		QStandardItem* item = this->itemFromIndex(index);
		item->setData(iconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
	}
	//else if (index.siblingAtColumn((int)ColunmsOrder::DATE_MODIDFIED).data().toString().isEmpty() &&
	//	!index.parent().isValid() && index.isValid())
	//{
	//	QStandardItem* item = this->item(rows - 1);
	//	item->setData(iconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
	//}

	for (int i = 0; i < rows; ++i)
		setIcons(this->index(i, 0, index), depth + 1);
}

QModelIndex QFileInfoModel::byPathRecursive(QStringList::const_iterator piece, 
	QStringList::const_iterator end, const QModelIndex& parent) const
{
	if (piece == end) return parent;

	if (parent.isValid())
	{
		int rows = this->rowCount(parent);
		for (size_t i = 0; i < rows; i++)
		{
			QString childData = this->index(i, 0, parent).data().toString();
			if (!QString::compare(*piece, childData, Qt::CaseInsensitive))
				return byPathRecursive(piece + 1, end, this->index(i, 0, parent));
		}
	}
	return parent;
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
	row.insert(int(ColunmsOrder::SIZE_BYTES), new QStandardItem(QString::number(drive.fileInfo().size())));

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
	row.insert(int(ColunmsOrder::ICON_NAME), new QStandardItem(iconProvider.icon(info).name()));
	row.insert(int(ColunmsOrder::DATE_CREATED), new QStandardItem(info.birthTime().toString()));
	row.insert(int(ColunmsOrder::GROUP), new QStandardItem(info.group()));
	row.insert(int(ColunmsOrder::OWNER), new QStandardItem(info.owner()));
	row.insert(int(ColunmsOrder::OWNER_ID), new QStandardItem(QString::number(info.ownerId())));
	row.insert(int(ColunmsOrder::CUSTOM_METHADATA), new QStandardItem(QString()));
	row.insert(int(ColunmsOrder::SIZE_BYTES), new QStandardItem(QString::number(info.size())));

	return row;
}
