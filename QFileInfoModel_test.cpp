#include "QFileInfoModel_test.h"

#include <QFileSystemModel>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QDir>

#include "system_depend_functions.h"



void QFileInfoModel_test::genFromGoogleDriveResponse()
{
	QByteArray val;
	QFile file;
	file.setFileName("tests/test-response.json");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		val = file.readAll();
		file.close();
	}
	QJsonDocument response = QJsonDocument::fromJson(val);
	QFileInfoModel model;
	model.genFromGoogleDriveResponse(response);

	QJsonArray arr = response["files"].toArray();
	QJsonValue first = arr[0], second = arr[1];

	// First
	QCOMPARE(model.index(0, (int)ColunmsOrder::NAME).data().toString(), first["name"].toString());
	QCOMPARE(model.index(0, (int)ColunmsOrder::SIZE).data().toString(), model.fileSize(first["size"].toString().toLongLong()));
	QMimeDatabase dt;
	QCOMPARE(model.index(0, (int)ColunmsOrder::TYPE).data().toString(), dt.mimeTypeForFile(QFileInfo(first["name"].toString())).comment());

	QCOMPARE(model.index(0, (int)ColunmsOrder::DATE_MODIDFIED).data().toString(), FromRfc3339(first["modifiedTime"].toString()).toString());
	QCOMPARE(model.index(0, (int)ColunmsOrder::ICON_NAME).data().toString(), first["mimeType"].toString());
	QCOMPARE(model.index(0, (int)ColunmsOrder::DATE_CREATED).data().toString(), FromRfc3339(first["createdTime"].toString()).toString());
	QCOMPARE(model.index(0, (int)ColunmsOrder::GROUP).data().toString(), "");
	// TODO: Finish comparing

	// Second
	QCOMPARE(model.index(1, (int)ColunmsOrder::NAME).data().toString(), second["name"].toString());
	QCOMPARE(model.index(1, (int)ColunmsOrder::SIZE).data().toString(), model.fileSize(second["size"].toString().toLongLong()));
	QCOMPARE(model.index(1, (int)ColunmsOrder::TYPE).data().toString(), dt.mimeTypeForFile(QFileInfo(second["name"].toString())).comment());

	QCOMPARE(model.index(1, (int)ColunmsOrder::DATE_MODIDFIED).data().toString(), FromRfc3339(second["modifiedTime"].toString()).toString());
	QCOMPARE(model.index(1, (int)ColunmsOrder::ICON_NAME).data().toString(), second["mimeType"].toString());
	QCOMPARE(model.index(1, (int)ColunmsOrder::DATE_CREATED).data().toString(), FromRfc3339(second["createdTime"].toString()).toString());
	QCOMPARE(model.index(1, (int)ColunmsOrder::GROUP).data().toString(), "");
	// TODO: Finish comparing
}

void QFileInfoModel_test::genExternalDrivesModel()
{
	QFileInfoModel model;
	model.genExternalDrivesModel(10);

	QFileSystemModel systemModel;
	auto removable = removableDrives();
	for (auto& drive : removable) {
		QVERIFY2(systemModel.index(drive).isValid(), "Test removableDrives() function accuracy");

		QModelIndex modelDriveIndex;
		for (size_t i = 0; i < model.rowCount(); ++i) {
			if (model.index(i, 0).data().toString().remove('\\').remove('/').remove(':') ==
				drive.remove('\\').remove('/').remove(':')) {
				modelDriveIndex = model.index(i, 0);
				break;
			}
		}
		QVERIFY(modelDriveIndex.isValid());

		//TODO: Check all subtrees
		//QVERIFY(compareRecursive(model.itemFromIndex(modelDriveIndex), systemModel);
	}
}

void QFileInfoModel_test::genStaticSystemModel()
{
	// TODO:
}

void QFileInfoModel_test::genGoogleDriveModel()
{
	// TODO:
}

void QFileInfoModel_test::insertFileLinkToTheFolder()
{
	QFileInfoModel model;
	model.genStaticSystemModel(2);
}

void QFileInfoModel_test::insertFileToTheFolder()
{
	// TODO:
}

void QFileInfoModel_test::appendFolder()
{
	// TODO:
}

void QFileInfoModel_test::deleteFile()
{
	// TODO:
}

void QFileInfoModel_test::setName()
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

		model.setName("test", index);
		
		QCOMPARE(item->data(0).toString(), "test");
	}
}

void QFileInfoModel_test::getPath()
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
		QFileInfo info(index.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString());
		auto path = model.getPath(index);
		QString pth; pth += path[0]  + path[1]; for (size_t i = 2; i < path.count(); i++) pth += "/" + path[i];

		QCOMPARE(pth, info.absoluteFilePath());
	}
}

void QFileInfoModel_test::getPathfFromInfo()
{
	// TODO:
}

void QFileInfoModel_test::byPath()
{
	constexpr size_t kMinStep = 3, kMaxStep = 100, kDepth = 2, kTestCount = 10;

	QFileInfoModel model;
	model.genStaticSystemModel(kDepth);

	for (size_t t = 0; t < kTestCount; t++)
	{

		QDirIterator it("", QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		QRandomGenerator rand;

		// Select random file
		QString file;
		while (file.isEmpty())
		{
			size_t j = 0, k = rand.bounded(kMinStep, kMaxStep);
			while (j < k && it.hasNext())
			{
				it.next();  ++j;
			}

			// qDebug() << it.filePath();
			if (it.filePath().count('\\') <= kDepth && it.filePath().count('/') <= kDepth)
				file = it.filePath();
		}

		auto path = QFileInfo(file).absoluteFilePath();
		auto index = model.byPath(path);
		QCOMPARE(index.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString(), path);
	}
}

void QFileInfoModel_test::pathFromStringList()
{
	QStringList list = { "C://", "test", "test.txt" };
	auto path = QFileInfoModel::pathFromStringList(list); path.remove(0, 1);
	QCOMPARE(path, tr("C:/test/test.txt"));

	list = { "C:\\\\", "test", "test.txt" };
	path = QFileInfoModel::pathFromStringList(list); path.remove(0, 1);
	QCOMPARE(path, tr("C:/test/test.txt"));

	list = { "C:\\\\", "test", "test.txt" };
	path = QFileInfoModel::pathFromStringList(list); path.remove(0, 1);
	QCOMPARE(path, tr("C:/test/test.txt"));
}

void QFileInfoModel_test::readFile()
{
	QFileInfoModel model;

	QVERIFY_THROWS_NO_EXCEPTION(model.readFile("tests/test-file.fsh"));
	QVERIFY_THROWS_EXCEPTION(std::runtime_error, model.readFile("tests/aaaaaaaa.kk"));
	QVERIFY_THROWS_EXCEPTION(std::runtime_error, model.readFile("tests/test-fail.fsh"));
}

void QFileInfoModel_test::writeFile()
{
	QFileInfoModel model;

	model.genStaticSystemModel(2);
	QVERIFY_THROWS_NO_EXCEPTION(model.writeFile("test.txt", 2));

	QFile file("test.txt");
	file.remove();
}

//bool QFileInfoModel_test::compareRecursive(QStandardItem* item1, QStandardItem* item2, size_t maxDepth, size_t curDepth)
//{
//    if (curDepth > maxDepth)
//        return true;
//
//    if (item1->data(0).toString() != item2->data(0).toString())
//        return false;
//
//    if (++curDepth <= maxDepth) {
//        if (item1->rowCount() == item2->rowCount()) {
//            bool isEqual = true;
//            for (size_t i = 0; i < item1->rowCount(); ++i)
//                isEqual &= compareRecursive(item1, item2, maxDepth, curDepth);
//            
//            return isEqual;
//        }
//        else
//            return false;
//    }
//    
//    return true;
//}

