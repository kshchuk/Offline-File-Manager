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
	CUSTOM_METHADATA
};


class QFileInfoModel  : public QStandardItemModel
{
	Q_OBJECT

public:
	QFileInfoModel(QObject *parent = nullptr);
	~QFileInfoModel();

	QList<QString> getPath(QModelIndex index) const;

	QAbstractItemModel* readFile(QString fileName);
	void writeFile(QString fileName, size_t maxDepth) const;

	QAbstractItemModel* genStaticSystemModel(size_t maxDepth);
	QAbstractItemModel* genExternalDrivesModel(size_t maxDepth);

	QModelIndex byPath(QString::iterator start,
		QString::iterator end, QModelIndex parent = QModelIndex()) const;

private:

	const size_t columnsNumber = 9;

	QMimeDatabase db;
	QFileIconProvider iconProvider;

	void readHierarchyRecursive(QModelIndex parent, const QString& path,
		size_t maxDepth, size_t curDepth = 1);

	QList<QStandardItem*> packDrive(const QDirIterator& drive) const;
	// List of columns:
	// Name | Size | Type | Date Modified | Icon Name | Birth Time | Group | Owner | OwnerID | Custom metadata
	QList<QStandardItem*> fromFileInfo(const QFileInfo& info) const;
	QList<QStandardItem*> packInfo(const QModelIndex& index) const;
	void initFileModelHeaders(QFileInfoModel* model) const;
	QString fileSize(const QFileInfo& info) const;
	void setIcons(const QModelIndex& index = QModelIndex(), int depth = 0);
};
