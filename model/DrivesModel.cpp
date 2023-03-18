#include "DrivesModel.h"

#include <QJsonArray>

#include "../system_depend_functions.h"
#include "../GoogleGateway.h"
#include "Record.h"



namespace manager
{
    QStorageInfo DrivesModel::storageInfo = QStorageInfo();

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


    QFileInfoModel* ExternalDrivesModel::generate(size_t maxDepth)
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

    QFileInfoModel* AllDrivesModel::generate(size_t maxDepth)
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

    QFileInfoModel* GoogleDriveModel::generate(size_t maxDepth)
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
		this->setIcons();
		emit loaded();
	}
}