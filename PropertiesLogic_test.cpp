/*#include "PropertiesLogic_test.h"

#include <QRandomGenerator>
#include <QTest>

#include "QFileInfoModel.h"
#include "PropertiesLogic.h"

void PropertiesLogic_test::saveMeta()
{
	constexpr size_t kDepth = 2, kTestCount = 10;

	QFileInfoModel model;
	model.genStaticSystemModel(kDepth);
	QRandomGenerator rand;

	for (size_t t = 0; t < kTestCount; t++)
	{
		QStandardItem* item;;

		// Select random file record at a certain depth

		bool isFound = true; // magic
		QModelIndex index;
		while (!index.isValid())
		{
			item = model.invisibleRootItem();
			for (size_t i = 0; i <= kDepth; i++)
			{
				if (!item->hasChildren()) {
					isFound = false;
					break;
				}
				item = item->child(rand.bounded(item->rowCount()));
			}
			if (isFound)
				index = item->index();
			else
				isFound = true; // another magic
		}

		PropertiesLogic log(index, &model);
		log.setCustomMethadata("Saving meta test");
		log.saveMeta();

        QCOMPARE(index.siblingAtColumn((int)ColunmsOrder::CUSTOM_METHADATA).data(257).toString(), "Saving meta test");
	}
}*/
