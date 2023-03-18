#include "QFileInfoModel.h"

#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QtCborCommon>
#include <QApplication>
#include <QException>
#include <QMimeType>
#include <QLocale>
#include <QStyle>

#include <cassert>

#include "ModelSerializer.h"
#include "Record.h"


namespace manager
{
    QFileIconProvider QFileInfoModel::iconProvider = QFileIconProvider();
    QFileSystemModel QFileInfoModel::system_model = QFileSystemModel();


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

    QList<QString> QFileInfoModel::getPath(QModelIndex index) const noexcept
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

    QString QFileInfoModel::getPathFromInfo(const QModelIndex& index) const noexcept
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
			setIcons();
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


	QModelIndex QFileInfoModel::appendVirtualFolder(QModelIndex parent)
	{
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

    QModelIndex QFileInfoModel::byPath(QString path) const noexcept
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

	QString FileInfoModelHelper::pathFromStringList(const QStringList& list)
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
		int rows = this->rowCount(index);

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
}
