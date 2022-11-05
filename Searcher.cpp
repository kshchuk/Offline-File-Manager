#include "Searcher.h"

QModelIndexList Searcher::search(QVariant criterias[], QString path)
{
	QModelIndexList indexList;
	QModelIndex index = model->byPath(path);

	return searchRecursive(criterias, index);
}

QModelIndexList Searcher::searchRecursive(QVariant criteria[], 
														const QModelIndex& index)
{
	bool success = false;
	if (criteria[NAME].isValid())
		success = index.data().toString().contains(criteria[NAME].toString(),
															Qt::CaseInsensitive);
	if (criteria[TYPE].isValid())
		success = success && QString::compare(criteria[TYPE].toString(),
			index.siblingAtColumn((int)ColunmsOrder::TYPE).data().toString(), Qt::CaseSensitive) == 0;
	
	if (criteria[HASH].isValid())
		success = success && QString::compare(criteria[HASH].toString(),
			index.siblingAtColumn((int)ColunmsOrder::MD5).data().toString(), Qt::CaseSensitive) == 0;

	if (criteria[CUSTOM_META].isValid())
		success = success && index.siblingAtColumn((int)ColunmsOrder::CUSTOM_METHADATA).data().toString(
											).contains(criteria[CUSTOM_META].toString(),Qt::CaseInsensitive);

	if (criteria[CREATION_TIME_MIN].isValid())
		success = success && criteria[CREATION_TIME_MIN].toDateTime() <=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_CREATED).data().toString());

	if (criteria[CREATION_TIME_MAX].isValid())
		success = success && criteria[CREATION_TIME_MAX].toDateTime() >=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_CREATED).data().toString());

	if (criteria[EDIT_TIME_MIN].isValid())
		success = success && criteria[EDIT_TIME_MIN].toDateTime() <=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_MODIDFIED).data().toString());

	if (criteria[EDIT_TIME_MAX].isValid())
		success = success && criteria[EDIT_TIME_MAX].toDateTime() >=
		QDateTime::fromString(index.siblingAtColumn((int)ColunmsOrder::DATE_MODIDFIED).data().toString());

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
