/*#include "Searcher_test.h"
#include "Searcher.h"

#include <QMimeType>
#include <QByteArray>
#include <QDirIterator>
#include <QMimeDatabase>
#include <QRandomGenerator>
#include <QCryptographicHash>


// TODO: Transfer to a separate class for utils etc.
QByteArray hash(const QFileInfo& info)
{
	QCryptographicHash crypto(QCryptographicHash::Md5);

	QByteArray qb; qlonglong size = info.size();
	qb.setNum(size); crypto.addData(qb);

	QString path = info.filePath();
	QFile file(path);

	const int steps = 10;
	const int chunkSize = 10000;
	const qlonglong chunkStep = size / steps;

	crypto.addData(file.read(chunkSize));
	file.seek(size - chunkSize);
	crypto.addData(file.read(chunkSize));

	if (chunkSize <= size)
	{
		qlonglong cur = chunkSize;
		file.seek(cur);
		for (size_t i = 0; i < steps - 2; ++i)
		{
			crypto.addData(file.read(chunkSize));
			if (chunkSize * i < size)
				file.seek(chunkSize * i);
			else
				break;
		}
	}

	QByteArray hash = crypto.result();
	file.close();
	return hash;
}

void Searcher_test::search_by_type()
{
	QFileInfoModel model;
	Searcher searcher(&model);

	model.genStaticSystemModel(kDepth);

	for (size_t i = 0; i < kTestCount; i++)
	{
		QDirIterator it(kTestPath, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
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

		if (!file.isEmpty())
		{
			QFileInfo info(file);
			QByteArray fileHash = hash(info);

			// Search by type
			QMimeDatabase db;
			QMimeType mime = db.mimeTypeForFile(info);
			QString fileType = mime.comment();
			QVariant *var = new QVariant[searcher.criteriasCount];
			var[searcher.TYPE] = fileType;

			QModelIndexList files = searcher.search(var, kTestPath);
			delete[] var;

			QVERIFY(!files.isEmpty());

			bool isFound = false;
			for (const auto& ifile : files)
			{
				if (hash(QFileInfo(ifile.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString())) == fileHash)
				{
					isFound = true;
					break;
				}
			}
			QVERIFY2(isFound, "Search by Type");
		}
	}
}

void Searcher_test::search_by_name()
{
	QFileInfoModel model;
	Searcher searcher(&model);

	model.genStaticSystemModel(kDepth);

	for (size_t i = 0; i < kTestCount; i++)
	{
		QDirIterator it(kTestPath, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
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

			qDebug() << it.filePath() << "ffffffffffffffffffffff";
			if (it.filePath().count('\\') <= kDepth && it.filePath().count('/') <= kDepth)
				file = it.filePath();
		}

		if (!file.isEmpty())
		{
			QFileInfo info(file);
			QByteArray fileHash = hash(info);

			// Search by name
			QVariant* var = new QVariant[searcher.criteriasCount];
			QString fileName = info.fileName();
			var[Searcher::NAME] = fileName;

			QModelIndexList files = searcher.search(var, kTestPath);
			delete[] var;

			QVERIFY(!files.isEmpty());

			int isFound = false;
			for (const auto& ifile : files)
			{
				if (hash(QFileInfo(ifile.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString())) == fileHash)
				{
					isFound = true;
					break;
				}
			}
			QVERIFY2(isFound, "Search by Name");
		}
	}
}

void Searcher_test::search_by_hash()
{
	QFileInfoModel model;
	Searcher searcher(&model);

	model.genStaticSystemModel(kDepth);

	for (size_t i = 0; i < kTestCount; i++)
	{
		QDirIterator it(kTestPath, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
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
			if (it.filePath().count('\\') <= kDepth && it.filePath().count('/') <= kDepth && !QFileInfo(it.fileInfo()).isDir())
				file = it.filePath();
		}

		if (!file.isEmpty())
		{
			QFileInfo info(file);
			QByteArray fileHash = hash(info);
			
			// Search by Hash
			auto var = new QVariant[searcher.criteriasCount];
			var[searcher.HASH] = fileHash;

			auto files = searcher.search(var, kTestPath);
			delete[] var;

			QVERIFY(!files.isEmpty());

			auto isFound = false;
			for (const auto& ifile : files)
			{
				if (hash(QFileInfo(ifile.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString())) == fileHash)
				{
					isFound = true;
					break;
				}
			}
			QVERIFY2(isFound, "Search by Hash");

		}
	}
}

void Searcher_test::search_by_minimum_creation_time()
{
	QFileInfoModel model;
	Searcher searcher(&model);

	model.genStaticSystemModel(kDepth);

	for (size_t i = 0; i < kTestCount; i++)
	{
		QDirIterator it(kTestPath, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
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

			//qDebug() << it.filePath();
			if (it.filePath().count('\\') <= kDepth && it.filePath().count('/') <= kDepth)
				file = it.filePath();
		}

		if (!file.isEmpty())
		{
			QFileInfo info(file);
			QByteArray fileHash = hash(info);


			// Search by minimum creation time
			auto var = new QVariant[searcher.criteriasCount];
			QDateTime minCreationTime = info.birthTime() - kTimeShift;
			var[searcher.CREATION_TIME_MIN] = minCreationTime;

			auto files = searcher.search(var, kTestPath);
			delete[] var;

			QVERIFY(!files.isEmpty());

			auto isFound = false;
			for (const auto& ifile : files)
			{
				if (hash(QFileInfo(ifile.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString())) == fileHash)
				{
					isFound = true;
					break;
				}
			}
			QVERIFY2(isFound, "Search by minimum creation time");

		}
	}
}

void Searcher_test::search_by_maximum_creationand_minimum_edited_time()
{
	QFileInfoModel model;
	Searcher searcher(&model);

	model.genStaticSystemModel(kDepth);

	for (size_t i = 0; i < kTestCount; i++)
	{
		QDirIterator it(kTestPath, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
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

		if (!file.isEmpty())
		{
			QFileInfo info(file);
			QByteArray fileHash = hash(info);


			// Search by maximum creation and minimum edited time
			auto var = new QVariant[searcher.criteriasCount];
			QDateTime maxCreationTime = info.birthTime() + kTimeShift;
			QDateTime minEditedTime = info.lastModified() - kTimeShift;

			var[searcher.CREATION_TIME_MAX] = maxCreationTime;
			var[searcher.EDIT_TIME_MIN] = minEditedTime;

			auto files = searcher.search(var, kTestPath);
			delete[] var;

			QVERIFY(!files.isEmpty());

			auto isFound = false;
			for (const auto& ifile : files)
			{
				if (hash(QFileInfo(ifile.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString())) == fileHash)
				{
					isFound = true;
					break;
				}
			}
			QVERIFY2(isFound, "Search by maximum creation and minimum edited time");
		}
	}
}*/
