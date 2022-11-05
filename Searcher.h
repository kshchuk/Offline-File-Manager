#pragma once
#include "QFileInfoModel.h"

#include <QDateTime>


//using Name = QString;
//using Type = QString;
//using CreationTime = QDateTime;
//using EditTime = QDateTime;
//using Hash = QString;
//using CustomMeta = QString;


class Searcher : public QObject
{
	Q_OBJECT


public:
	Searcher(QFileInfoModel* model) : model(model) {};

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

	const size_t criteriasCount = 8;

	QModelIndexList search(QVariant criterias[], QString path);


signals:
	void found(QModelIndex);
	void finished(QModelIndexList);


private:
	const QFileInfoModel* model;

	QModelIndexList searchRecursive(QVariant criterias[], const QModelIndex& index);
};

