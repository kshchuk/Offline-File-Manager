#pragma once

#include <QList>

#include "QFileInfoModel.h"


namespace manager {

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

    QList<manager::Record*> search(QVariant criterias[], QString path);

signals:
    void found(const manager::Record*);

private:
    QFileInfoModel* model;

    QList<manager::Record*> searchRecursive(QVariant criterias[], Record* record);
};
}

