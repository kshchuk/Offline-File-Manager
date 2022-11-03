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

#include "ModelSerializer.h"
#include "system_depend_functions.h"


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
	SIZE_BYTES
};

const size_t columnsNumber = 10;


const QString virtualFolderType = "virtual folder";


class QFileInfoModel  : public QStandardItemModel
{
	Q_OBJECT

public:
	QFileInfoModel(QObject *parent = nullptr);
	~QFileInfoModel();

	QList<QString> getPath(QModelIndex index) const;

	QModelIndex appendFolder(const QFileInfo& info, QModelIndex parent);
	void setName(QString newName, QModelIndex index);
	QModelIndex byPath(QString path) const;
	quint64 fileSize(const QModelIndex& index) const;

	void setFileName(const QString&);
	void setMaxDepth(size_t);

public slots:
	void readFile(QString fileName, size_t maxDepth);
	void writeFile(QString fileName);
	void genStaticSystemModel(size_t maxDepth);
	void genExternalDrivesModel(size_t maxDepth);

signals:
	void finished();
	void error(QString err);

	friend class WriteThread;
	friend class ReadThread;
	friend class ExternalDrivesGenThread;
	friend class SystemModelGenThread;


private:
	QMimeDatabase db;
	QFileIconProvider iconProvider;

	QString fileName;
	size_t maxDepth = 10;

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
	QModelIndex byPathRecursive(QStringList::const_iterator piece,
		QStringList::const_iterator end, const QModelIndex& parent) const;
};


class WriteThread : public QThread
{
	Q_OBJECT

private:
	QFileInfoModel* model;
	QString fileName;

public:
	WriteThread(QFileInfoModel* model, QString fileName) 
		: model(model), fileName(fileName) {}

	void run() override {		

		if (model->fileName.isEmpty()) {
			emit error(QString("Reading error: Name is empty"));
			emit finished();
			return;
		}

		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly)) {
			QDataStream stream(&file);

			ModelSerializer<> ser;
			Status st = ser.save(stream, model);
			emit error(QString("Writing error:" + fileName));

			file.flush();
			file.close();
		}
		else {
			emit error(QString("Unable to open file:  " + fileName));
		}
		emit finished();
	}

signals:
	void error(QString e);
	void finished();
};


class ReadThread : public QThread
{
	Q_OBJECT

private:
	QFileInfoModel* model;
	QString fileName;
	size_t maxDepth;

public:
	ReadThread(QFileInfoModel* model, QString fileName, size_t maxDepth) 
		: model(model), fileName(fileName), maxDepth(maxDepth) {}

	void run() override {
		if (fileName.isEmpty()) {
			emit error(QString("Reading error: Name is empty"));
		}

		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly)) {
			QDataStream stream(&file);
			model->clear();
			ModelSerializer<> ser;
			Status st = ser.load(stream, model);
			if (!st.ok()) {
				emit error(QString("Reading error: " + fileName));
			}
			model->initFileModelHeaders(model);

			model->setIcons();

			file.close();
		}
		else {
			emit error(QString("Unable to open file: " + fileName));
		}

		emit finished();
	}

signals:
	void error(QString e);
	void finished();
};

class ExternalDrivesGenThread : public QThread
{
	Q_OBJECT

private:
	QFileInfoModel* model;
	size_t maxDepth;

public:
	ExternalDrivesGenThread(QFileInfoModel* model, size_t maxDepth)
		: model(model), maxDepth(maxDepth) {}

	void run() override {
		model->clear();

		QStandardItem* root = model->invisibleRootItem();

		QList<QString> externalDrives = removableDrives();
		foreach(auto drive, externalDrives)
		{
			QDirIterator it(drive);

			QList<QStandardItem*> preparedRow = model->packDrive(it);
			root->appendRow(preparedRow);

			int row = root->rowCount() - 1;
			QModelIndex index = root->child(row)->index();

			model->readHierarchyRecursive(index, it.path(), maxDepth);
		}
		model->initFileModelHeaders(model);
		emit finished();
	}

signals:
	void error(QString e);
	void finished();
};


class SystemModelGenThread : public QThread
{
	Q_OBJECT

private:
	QFileInfoModel* model;
	size_t maxDepth;

public:
	SystemModelGenThread(QFileInfoModel* model, size_t maxDepth)
		: model(model), maxDepth(maxDepth) {}

	void run() override {
		model->clear();

		foreach(auto drive, QDir::drives())
		{
			QDirIterator it(drive.absoluteFilePath());

			QList<QStandardItem*> preparedRow = model->packDrive(it);
			model->appendRow(preparedRow);

			int row = model->rowCount() - 1;
			QModelIndex index = model->index(row, 0);
			//root->child(row)->index();

			model->readHierarchyRecursive(index, it.path(), maxDepth);
		}
		model->initFileModelHeaders(model);
		emit finished();
	}

signals:
	void error(QString e);
	void finished();
};