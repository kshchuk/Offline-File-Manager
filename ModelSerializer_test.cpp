#include "ModelSerializer_test.h"

#include <QFile>
#include <QDataStream>
#include <qassert.h>

void ModelSerializer_test::save()
{
	QStandardItemModel model;
	model.setHeaderData(0, Qt::Horizontal, tr("Col 1"));
	model.setHeaderData(1, Qt::Horizontal, tr("Col 2"));
	model.appendRow(QList<QStandardItem*>{new QStandardItem("0 0"), new QStandardItem("0 1")});
	model.appendRow(QList<QStandardItem*>{new QStandardItem("1 0"), new QStandardItem("1 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  0 0"), new QStandardItem("(1 0) (0 1) -> 0 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  1 0"), new QStandardItem("(1 0) (1 1) -> 0 1")});

	QFile file("test.tmp");
	file.open(QIODevice::WriteOnly);
	QDataStream ostream(&file);
	ModelSerializer<> ser;
	Status status = ser.save(ostream, &model);
	file.flush();

	QVERIFY(status.ok());

	file.close();
	file.remove();
}


void ModelSerializer_test::load()
{
	QStandardItemModel model;
	model.setHeaderData(0, Qt::Horizontal, tr("Col 1"));
	model.setHeaderData(1, Qt::Horizontal, tr("Col 2"));
	model.appendRow(QList<QStandardItem*>{new QStandardItem("0 0"), new QStandardItem("0 1")});
	model.appendRow(QList<QStandardItem*>{new QStandardItem("1 0"), new QStandardItem("1 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  0 0"), new QStandardItem("(1 0) (0 1) -> 0 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  1 0"), new QStandardItem("(1 0) (1 1) -> 0 1")});

	QFile file("test.tmp");
	file.open(QIODevice::WriteOnly);
	QDataStream stream(&file);
	ModelSerializer<> ser;
	Status status = ser.save(stream, &model);
	file.flush();
	file.close();

	Q_ASSERT(status.ok());


	// READING
	QStandardItemModel model2;
	file.open(QIODevice::ReadOnly);
	stream.resetStatus();
	status = ser.load(stream, &model2);

	QVERIFY(status.ok());
	QCOMPARE(model2.columnCount(), model.columnCount());
	QCOMPARE(model2.rowCount(), model.rowCount());
	for (size_t i = 0; i < model.columnCount(); ++i)
		QCOMPARE(model2.headerData(i, Qt::Horizontal).toString(), model.headerData(i, Qt::Horizontal).toString());
	
	QCOMPARE(model2.item(0, 0)->data().toString(), model.item(0, 0)->data().toString());
	QCOMPARE(model2.item(0, 1)->data().toString(), model.item(0, 1)->data().toString());

	QCOMPARE(model2.item(1, 0)->data().toString(), model.item(1, 0)->data().toString());
	QCOMPARE(model2.item(1, 1)->data().toString(), model.item(1, 1)->data().toString());

	QCOMPARE(model2.item(1, 0)->child(0, 0)->data().toString(), model.item(1, 0)->child(0, 0)->data().toString());
	QCOMPARE(model2.item(1, 0)->child(0, 1)->data().toString(), model.item(1, 0)->child(0, 1)->data().toString());
	QCOMPARE(model2.item(1, 0)->child(1, 0)->data().toString(), model.item(1, 0)->child(1, 0)->data().toString());
	QCOMPARE(model2.item(1, 0)->child(1, 1)->data().toString(), model.item(1, 0)->child(1, 1)->data().toString());

	file.close();
	file.remove();
}
