#include "Searcher.h"

namespace manager
{

	QList<manager::Record *> Searcher::search(QVariant criterias[], QString path)
	{
		QList<manager::Record *> recordList;
		QModelIndex index = model->byPath(path);

		if (index.isValid())
		{
			recordList = searchRecursive(criterias, dynamic_cast<Record *>(model->itemFromIndex(index)));
			return recordList;
		}
		else
		{
			QList<manager::Record *> list;
			int rows = model->rowCount();
			for (size_t i = 0; i < rows; ++i)
				list += searchRecursive(criterias, dynamic_cast<Record *>(model->itemFromIndex(model->index(i, 0))));
			return list;
		}
	}

	QList<manager::Record *> Searcher::searchRecursive(QVariant criteria[], Record* record)
	{
		bool success = true;

		if (criteria[NAME].isValid())
			success = success && record->getName().contains(criteria[NAME].toString(), Qt::CaseInsensitive);

		if (criteria[TYPE].isValid())
			success = success && record->getType().contains(criteria[TYPE].toString(), Qt::CaseInsensitive);

		if (criteria[HASH].isValid())
			success = success && !QString::compare(criteria[HASH].toString(), record->getHash(), Qt::CaseSensitive);

		if (criteria[CUSTOM_META].isValid())
			success = success && record->getCustomMetadata().contains(criteria[CUSTOM_META].toString(), Qt::CaseInsensitive);

		if (criteria[CREATION_TIME_MIN].isValid())
			success = success && (criteria[CREATION_TIME_MIN].toDateTime() <=
								  QDateTime::fromString(record->getDateCreated()));

		if (criteria[CREATION_TIME_MAX].isValid())
			success = success && (criteria[CREATION_TIME_MAX].toDateTime() >=
								  QDateTime::fromString(record->getDateCreated()));

		if (criteria[EDIT_TIME_MIN].isValid())
			success = success && (criteria[EDIT_TIME_MIN].toDateTime() <=
								  QDateTime::fromString(record->getDateModified()));

		if (criteria[EDIT_TIME_MAX].isValid())
			success = success && (criteria[EDIT_TIME_MAX].toDateTime() >=
								  QDateTime::fromString(record->getDateModified()));

		QList<Record*> list;
		if (success)
		{
			emit found(record);
			list.append(record);
		}

		if (record->hasChildren())
		{
			for (size_t i = 0; i < record->rowCount(); ++i)
			{
				list += searchRecursive(criteria, dynamic_cast<Record *>(record->child(i)));
			}
		}
		return list;
	}
}