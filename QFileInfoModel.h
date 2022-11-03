#pragma once

#include <QStandardItemModel>
#include <QFileIconProvider>
#include <QMimeDatabase>
#include <QStandardItem>
#include <QDirIterator>
#include <QFileInfo>
#include <QString>
#include <QList>
#include <QIcon>


enum class ColunmsOrder
{
	NAME,
	SIZE,
	TYPE,
	DATE_MODIDFIED,
	ICON_NAME,
	DATE_CREATED,
	GROUP,
	OWNER,
	OWNER_ID,
	CUSTOM_METHADATA,
	SIZE_BYTES,
	FULL_PATH
};

const size_t columnsNumber = 11;


static const QString virtualFolderType = "virtual folder";
static const QString linkToFileType = "link to file/folder";


class QFileInfoModel  : public QStandardItemModel
{
	Q_OBJECT

public:
	QFileInfoModel(QObject *parent = nullptr);
	~QFileInfoModel();

	QList<QString> getPath(QModelIndex index) const;
	QString getPathfFromInfo(const QModelIndex &index) const;

	QAbstractItemModel* readFile(QString fileName);
	void writeFile(QString fileName, size_t maxDepth) const;
	void deleteFile(const QModelIndex& index);

	QAbstractItemModel* genStaticSystemModel(size_t maxDepth);
	QAbstractItemModel* genExternalDrivesModel(size_t maxDepth);

	QModelIndex appendFolder(const QFileInfo& info, QModelIndex parent);
	void setName(QString newName, QModelIndex index);
	QModelIndex byPath(QString path) const;
	quint64 fileSize(const QModelIndex& index) const;
	void insertFileLinkToTheFolder(QModelIndex toInsert, QModelIndex destination);

	static QString pathFromStringList(const QStringList& list);
	static bool isLink(const QModelIndex& index);
	static bool isFolder(const QModelIndex& index);


private:
	QMimeDatabase db;
	QFileIconProvider iconProvider;

	void readHierarchyRecursive(QModelIndex parent, const QString& path,
		size_t maxDepth, size_t curDepth = 1);

	QList<QStandardItem*> packDrive(const QDirIterator& drive) const;
	// List of columns:
	// Name | Size | Type | Date Modified | Icon Name | Birth Time | Group | Owner | OwnerID | Custom metadata
	QList<QStandardItem*> fromFileInfo(const QFileInfo& info) const;
	QList<QStandardItem*> packLink(const QModelIndex& index) const;
	void initFileModelHeaders(QFileInfoModel* model) const;
	QString fileSize(const QFileInfo& info) const;
	void setIcons(const QModelIndex& index = QModelIndex(), int depth = 0);
	QModelIndex byPathRecursive(QStringList::const_iterator piece,
		QStringList::const_iterator end, const QModelIndex& parent) const;
};
