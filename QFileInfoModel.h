#pragma once

#include <QStandardItemModel>
#include <QVariantMap>
#include <QFileInfo>
#include <QString>

class QFileInfoModel  : public QStandardItemModel
{
	Q_OBJECT

public:
	QFileInfoModel(QObject *parent = nullptr);
	~QFileInfoModel();

	QAbstractItemModel* readFile(QString fileName);
	void writeFile(QString fileName, size_t maxDepth) const;

	QAbstractItemModel* genStaticSystemModel(size_t maxDepth);
	QAbstractItemModel* genExternalDrivesModel(size_t maxDepth);

private:
	void readHierarchyRecursive(QModelIndex parent, const QString& path,
		size_t maxDepth, size_t curDepth = 1);
	QVariantMap fromFileInfo(const QFileInfo& info) const;
	void initFileModelHeaders(QFileInfoModel* model) const;
};
