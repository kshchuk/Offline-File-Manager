#include "QFileInfoModel.h"

#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QtCborCommon>
#include <QApplication>
#include <QException>
#include <QMimeType>
#include <QLocale>
#include <QStyle>

#include <functional>
#include <cassert>

#include "ModelSerializer.h"
#include "GoogleGateway.h"
#include "system_depend_functions.h"


namespace model
{

	QFileInfoModel::QFileInfoModel(QObject* parent)
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

	inline QString QFileInfoModel::getPathfFromInfo(const QModelIndex& index) const
	{
		return dynamic_cast<Record*>(this->itemFromIndex(index))->getFullPath();
	}

	QAbstractItemModel* QFileInfoModel::readFile(QString fileName)
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly)) {
			QDataStream stream(&file);
			this->clear();
			ModelSerializer<> ser;
			Status st = ser.load(stream, this);
			//setIcons();
			if (!st.ok()) throw std::runtime_error("Reading error:" + fileName.toStdString());
			this->initHeaders();


			file.close();
			return this;
		}
		else
			throw std::runtime_error("Unable to open file:" + fileName.toStdString());

		emit loaded();
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

	void QFileInfoModel::deleteFile(const QModelIndex& index)
	{
		this->removeRow(index.row(), index.parent());
	}

	bool QFileInfoModel::isLink(const QModelIndex& index) const
	{
		return typeid(this->itemFromIndex(index)) == typeid(Link*);
	}

	bool QFileInfoModel::isFolder(const QModelIndex& index) const
	{
		return typeid(this->itemFromIndex(index)) == typeid(Folder*);
	}


	QModelIndex QFileInfoModel::appendVirtualFolder(const QFileInfo& info, QModelIndex parent)
	{
		//QList<QStandardItem*> erow = fromFileInfo(info);
		//erow[(int)ColunmsOrder::NAME]->setData(iconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
		//erow[(int)ColunmsOrder::ICON_NAME]->setData(iconProvider.icon(QFileIconProvider::Folder).name(), Qt::DisplayRole);
		//erow[(int)ColunmsOrder::TYPE]->setData(virtualFolderType, Qt::DisplayRole);

		if (parent.isValid())
		{
			QStandardItem* iparent = this->itemFromIndex(parent);
			iparent->appendRow(new VirtualFolder("virtual"));
			int row = iparent->rowCount() - 1;
			return iparent->child(row)->index();
		}
		else {
			this->appendRow(new VirtualFolder("virtual"));
			int row = this->rowCount() - 1;
			return this->index(row, 0);
		}
	}

	void QFileInfoModel::setName(QString newName, QModelIndex index)
	{
		dynamic_cast<Record*>(this->itemFromIndex(index))->setName(newName);
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
				sum = dynamic_cast<Record*>(item)->getSizeInBytes().toLongLong();
			}
		}
		else {
			int rows = this->rowCount();
			for (size_t i = 0; i < rows; ++i)
				sum = fileSize(this->index(i, 0));
		}
		return sum;
	}

	void QFileInfoModel::insertFileLinkToTheFolder(QModelIndex toInsert, QModelIndex destination)
	{
		QStandardItem* dest = this->itemFromIndex(destination);
		dest->appendRow(new Link(toInsert));
	}

	void QFileInfoModel::insertFileToTheFolder(const QString& path, QModelIndex destination)
	{
		QStandardItem* dest = this->itemFromIndex(destination);
		QFileInfo info(path);
		
		if (info.isDir())
			dest->appendRow(new Folder(info));
		else
			dest->appendRow(new File(info));
	}

	QString QFileInfoModel::pathFromStringList(const QStringList& list)
	{
		QString spath;
		foreach(auto file, list) {
			file.removeIf([](QChar c) {return c == QChar('/') || c == QChar('\\'); });
			spath += '/' + file;
		};
		return spath;
	}

	void QFileInfoModel::setIcons(const QModelIndex& index, int depth)
	{
		if (index.isValid())
		{
			QString name = index.data().toString();
			Record* item = dynamic_cast<Record*>(this->itemFromIndex(index));

			QFileInfo info(name);

			if (typeid(item) == typeid(Drive*)) item->setIcon(iconProvider.icon(QFileIconProvider::Drive));
			else if (typeid(item) == typeid(VirtualFolder*) || typeid(item) == typeid(Folder*)) item->setIcon(iconProvider.icon(QFileIconProvider::Folder));
			else if (typeid(item) == typeid(Link*)) item->setIcon(iconProvider.icon(QFileInfo("example.lnk")));		
			else
				item->setData(iconProvider.icon(info), Qt::DecorationRole);
		}

		// if ((index.flags() & Qt::ItemNeverHasChildren) || !this->hasChildren(index));

		int rows = this->rowCount(index);

		// Folders
		//if (this->hasChildren(index) && index.parent().isValid()) {
		//	QStandardItem* item = this->itemFromIndex(index);
		//	item->setData(iconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
		//}hf

		for (int i = 0; i < rows; ++i)
			setIcons(this->index(i, 0, index), depth + 1);
	}

	void QFileInfoModel::initHeaders()
	{
		for (size_t i = 0; i < system_model.columnCount(); i++)
		{
			QStandardItem* header = new QStandardItem();
			header->setText(system_model.headerData(i, Qt::Horizontal).toString());
			this->setHorizontalHeaderItem(i, header);
		}
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


	QDateTime FromRfc3339(const QString& s) {
		auto datetime = s.split("T");
		datetime.append(datetime[1].split(".")); datetime.removeAt(1); datetime.removeLast();
		QString str; foreach(const auto & t, datetime) str += t + " ";
		auto date = datetime[0].split("-");
		auto time = datetime[1].split(":");
		QDateTime dt(QDate(date[0].toInt(), date[1].toInt(), date[2].toInt()), QTime(time[0].toInt(), time[1].toInt(), time[2].toInt()));
		return dt;
	}

	Record::Record(const QStandardItem& item)
		: QStandardItem(item)
	{
		assert(item.columnCount() == 13);
		
		this->size = item.model()->itemFromIndex(item.index().siblingAtColumn(1));
		this->type = item.model()->itemFromIndex(item.index().siblingAtColumn(2));
		this->dateModified = item.model()->itemFromIndex(item.index().siblingAtColumn(3));
		this->iconName = item.model()->itemFromIndex(item.index().siblingAtColumn(4));
		this->dateCreated = item.model()->itemFromIndex(item.index().siblingAtColumn(5));
		this->group = item.model()->itemFromIndex(item.index().siblingAtColumn(6));
		this->owner = item.model()->itemFromIndex(item.index().siblingAtColumn(7));
		this->ownerId = item.model()->itemFromIndex(item.index().siblingAtColumn(8));
		this->sizeInBytes = item.model()->itemFromIndex(item.index().siblingAtColumn(9));
		this->fullPath = item.model()->itemFromIndex(item.index().siblingAtColumn(10));
		this->hash = item.model()->itemFromIndex(item.index().siblingAtColumn(11));
		this->customMetadata = item.model()->itemFromIndex(item.index().siblingAtColumn(12));
	}

	Record::Record(const QIcon& icon, const QString& name)
		: QStandardItem(icon, name)

	{
		this->appendColumn({
			size,
			type,
			dateModified,
			iconName,
			dateCreated,
			group,
			owner,
			ownerId,
			sizeInBytes,
			fullPath,
			hash,
			customMetadata 
			});

		size = new QStandardItem();
		type = new QStandardItem();
		dateModified = new QStandardItem();
		iconName = new QStandardItem();
		dateCreated = new QStandardItem();
		group = new QStandardItem();
		owner = new QStandardItem();
		ownerId = new QStandardItem();
		sizeInBytes = new QStandardItem();
		fullPath = new QStandardItem();
		hash = new QStandardItem();
		customMetadata = new QStandardItem();
	}

	QString Record::fileSize(const QFileInfo& info)
	{
		if (info.isFile())
		{
			quint64 size = info.size();
			return locale.formattedDataSize(size);
		}
		return QString();
	}

	inline QString Record::fileSize(qint64 size)
	{
		return locale.formattedDataSize(size);
	}

	QByteArray File::hashFile(const QFileInfo& info)
	{
		QByteArray qb; qlonglong size = info.size();
		qb.setNum(size); crypto.addData(qb);

		QString path = info.filePath();
		QFile file(path);

		const int steps = 10;
		const int chunkSize = 10000;
		const qlonglong chunkStep = size / steps;

		crypto.addData(file.read(chunkSize));
		file.seek(size - chunkSize);
		crypto.addData(file.read(chunkSize));

		if (chunkSize <= size)
		{
			qlonglong cur = chunkSize;
			file.seek(cur);
			for (size_t i = 0; i < steps - 2; ++i)
			{
				crypto.addData(file.read(chunkSize));
				if (chunkSize * i < size)
					file.seek(chunkSize * i);
				else
					break;
			}
		}

		QByteArray hash = crypto.result();
		file.close();
		return hash;
	}
	

	Drive::Drive(const QDirIterator& drive)
		: Record(iconProvider.icon(QFileIconProvider::Drive), drive.fileName())
	{
		size->setText(fileSize(drive.fileInfo()));
		type->setText(mimeDatabase.mimeTypeForFile(drive.path()).comment());
		dateModified->setText(drive.fileInfo().lastModified().toString());
		iconName->setText(mimeDatabase.mimeTypeForFile(drive.path()).iconName());
		dateCreated->setText(drive.fileInfo().birthTime().toString());
		group->setText(drive.fileInfo().group());
		owner->setText(drive.fileInfo().owner());
		ownerId->setText(QString::number(drive.fileInfo().ownerId()));
		sizeInBytes->setText(QString::number(drive.fileInfo().size()));
		fullPath->setText(drive.fileInfo().absoluteFilePath());
	}

	Folder::Folder(const QFileInfo& folder)
		: Record(iconProvider.icon(QFileIconProvider::Folder), folder.fileName())
	{
		size->setText(fileSize(folder));
		type->setText(mimeDatabase.mimeTypeForFile(folder).comment());
		dateModified->setText(folder.lastModified().toString());
		iconName->setText(iconProvider.icon(QFileIconProvider::Folder).name());
		dateCreated->setText(folder.birthTime().toString());
		group->setText(folder.group());
		owner->setText(folder.owner());
		ownerId->setText(QString::number(folder.ownerId()));
		sizeInBytes->setText(QString::number(folder.size()));
		fullPath->setText(folder.absoluteFilePath());
	}

	File::File(const QFileInfo& file)
		: Record(iconProvider.icon(file), file.fileName())
	{
		size->setText(fileSize(file));
		type->setText(mimeDatabase.mimeTypeForFile(file).comment());
		dateModified->setText(file.lastModified().toString());
		iconName->setText(iconProvider.icon(file).name());
		dateCreated->setText(file.birthTime().toString());
		group->setText(file.group());
		owner->setText(file.owner());
		ownerId->setText(QString::number(file.ownerId()));
		sizeInBytes->setText(QString::number(file.size()));
		fullPath->setText(file.absoluteFilePath());
		hash->setText(hashFile(file));
	}

	Link::Link(const QModelIndex& index)
		:	Record(*this->model()->itemFromIndex(index))

	{
		this->setIcon(iconProvider.icon(QFileInfo("example.lnk")));
		this->type->setText(linkToFileType);
	}

	VirtualFolder::VirtualFolder(const QString& name)
		: Record(iconProvider.icon(QFileIconProvider::Folder), name)
	{
		type->setText(virtualFolderType);
		dateCreated->setText(QDateTime::currentDateTime().toString());
	}

	NetworkFile::NetworkFile(const QJsonValue& file)
		: Record(iconProvider.icon(QFileInfo(file["name"].toString())), file["name"].toString())
	{
		size->setText(fileSize(file["size"].toString().toLongLong()));
		type->setText(mimeDatabase.mimeTypeForFile(QFileInfo(file["name"].toString())).comment());
		dateModified->setText(FromRfc3339(file["modifiedTime"].toString()).toString());
		iconName->setText(file["mimeType"].toString());
		dateCreated->setText(FromRfc3339(file["createdTime"].toString()).toString());
		owner->setText(file["owners"][0]["emailAddress"].toString());
		ownerId->setText(file["permissionId"].toString());
		sizeInBytes->setText(file["size"].toString());
		fullPath->setText(file["webViewLink"].toString());
		hash->setText(file["md5Checksum"].toString());
	}

	void DrivesModel::readFilesRecursive(QModelIndex parent, const QString& path, size_t maxDepth, size_t curDepth)
	{
		if (curDepth > maxDepth)
			return;

		QStandardItem* parentItem = this->itemFromIndex(parent);

		QDirIterator it(path, QDir::NoDotAndDotDot | QDir::AllEntries);
		while (it.hasNext() && isRunning)
		{
			it.next();
			emit currentReadingFile(it.filePath());

			auto info = it.fileInfo();
			if (info.isDir())
				parentItem->appendRow(new Folder(info));
			else
				parentItem->appendRow(new File(info));

			readSize += info.size();
			emit fileRead(100 * readSize / allSize);

			int row = parentItem->rowCount() - 1;
			QModelIndex index = parentItem->child(row)->index();


			if (info.isDir())
				readFilesRecursive(index, it.filePath(), maxDepth, curDepth + 1);
		}
	}


	ExternalDrivesModel* ExternalDrivesModel::generate(size_t maxDepth)
	{
		this->clear();
		allSize = 0; readSize = 0; isRunning = true;
		QStandardItem* root = this->invisibleRootItem();

		this->initHeaders();
		QList<QString> externalDrives = removableDrives();
		foreach(auto drive, externalDrives) {
			storageInfo.setPath(drive);
			allSize += storageInfo.bytesTotal() - storageInfo.bytesFree();
		}

		foreach(auto drive, externalDrives)
		{
			QDirIterator it(drive);

			root->appendRow(new Drive(it));

			int row = root->rowCount() - 1;
			QModelIndex index = root->child(row)->index();

			readFilesRecursive(index, it.path(), maxDepth);
		}
		emit loaded();
		return this;
	}

	AllDrivesModel* model::AllDrivesModel::generate(size_t maxDepth)
	{
		this->clear();
		this->initHeaders();
		allSize = 0; readSize = 0; isRunning = true;
		foreach(auto drive, QDir::drives()) {
			storageInfo.setPath(drive.filePath());
			allSize += storageInfo.bytesTotal() - storageInfo.bytesFree();
		}

		foreach(auto drive, QDir::drives())
		{
			QDirIterator it(drive.absoluteFilePath());

			this->appendRow(new Drive(it));

			int row = this->rowCount() - 1;
			QModelIndex index = this->index(row, 0);

			readFilesRecursive(index, it.path(), maxDepth);
		}
		emit loaded();
		return this;
	}

	GoogleDriveModel* GoogleDriveModel::generate(size_t maxDepth)
	{
		this->clear();
		this->initHeaders();

		GoogleGateway* gateway = new GoogleGateway(this);
		connect(gateway, &GoogleGateway::authorized, gateway, &GoogleGateway::loadFileListPage);
		connect(gateway, &GoogleGateway::loadedFileListPage, this, &GoogleDriveModel::generateFromResponse);
		gateway->authorize();

		return this;
	}

	void GoogleDriveModel::generateFromResponse(const QJsonDocument& response)
	{
		QJsonArray files = response["files"].toArray();
		foreach(const QJsonValue & file, files)
		{
			qDebug() << file["name"].toString();
			this->invisibleRootItem()->appendRow(new NetworkFile(file));
		}
		// HACK: Don't update all icons when a new page is added
		//this->setIcons();
		emit loaded();
	}
}
