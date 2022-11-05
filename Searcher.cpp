#include "Searcher.h"

QModelIndexList Searcher::search(QVariant criterias[], QString path)
{
	QModelIndexList indexList;
	QModelIndex index = model->byPath(path);

	if (index.isValid()) {
		indexList = searchRecursive(criterias, index);
		emit finished(indexList);
		return indexList;
	}
	else {
		QModelIndexList list;
		int rows = model->rowCount();
		for (size_t i = 0; i < rows; ++i)
			list += searchRecursive(criterias, model->index(i, 0));
		return list;
	}
}

QModelIndexList Searcher::searchRecursive(QVariant criteria[], 
														const QModelIndex& index)
{
	bool success = true;
	if (criteria[NAME].isValid())
		success = success && index.data().toString().contains(criteria[NAME].toString(),
															Qt::CaseInsensitive);
	if (criteria[TYPE].isValid())
		success = success && index.siblingAtColumn((int)ColunmsOrder::TYPE).data().toString().contains(
			criteria[TYPE].toString(), Qt::CaseInsensitive);
	
	if (criteria[HASH].isValid())
		success = success && !QString::compare(criteria[HASH].toString(),
			index.siblingAtColumn((int)ColunmsOrder::MD5).data().toString(), Qt::CaseSensitive);

	if (criteria[CUSTOM_META].isValid())
		success = success && index.siblingAtColumn((int)ColunmsOrder::CUSTOM_METHADATA).data().toString().contains(
			criteria[CUSTOM_META].toString(), Qt::CaseInsensitive);

	if (criteria[CREATION_TIME_MIN].isValid())
		success = success && (criteria[CREATION_TIME_MIN].toDateTime() <=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_CREATED).data().toString()));

	if (criteria[CREATION_TIME_MAX].isValid())
		success = success && (criteria[CREATION_TIME_MAX].toDateTime() >=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_CREATED).data().toString()));

	if (criteria[EDIT_TIME_MIN].isValid())
		success = success && (criteria[EDIT_TIME_MIN].toDateTime() <=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_MODIDFIED).data().toString()));

	if (criteria[EDIT_TIME_MAX].isValid())
		success = success && (criteria[EDIT_TIME_MAX].toDateTime() >=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_MODIDFIED).data().toString()));

	QModelIndexList list;
	if (success) {
		emit found(index);
		list.append(index);
	}

	QStandardItem* item = model->itemFromIndex(index);
	if (item->hasChildren()) {
		for (size_t i = 0; i < item->rowCount(); ++i) {
			list += searchRecursive(criteria, item->child(i)->index());
		}
	}
	return list;
}
