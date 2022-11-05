#pragma once
#include "QFileInfoModel.h"

#include <QDateTime>


//using Name = QString;
//using Type = QString;
//using CreationTime = QDateTime;
//using EditTime = QDateTime;
//using Hash = QString;
//using CustomMeta = QString;


class Searcher : QObject
{
	Q_OBJECT

signals:
	void found(QModelIndex);

	enum CriteriaOrder
	{
		NAME,
		TYPE,
		HASH,
		CUSTOM_META,
		CREATION_TIME_MIN,
		CREATION_TIME_MAX,
		EDIT_TIME_MIN,
		EDIT_TIME_MAX
	};

public:
	QModelIndexList search(QVariant criterias[], QString path);


private:
	const QFileInfoModel* model;

	QModelIndexList searchRecursive(QVariant criterias[], const QModelIndex& index);
};

