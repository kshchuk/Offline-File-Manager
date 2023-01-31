#include "ModelSerializer_test.h"

#include <QDataStream>
#include <QTemporaryFile>
#include <qassert.h>

void ModelSerializer_test::save_loadHeaders()
{
	QStandardItemModel model;
	model.setHeaderData(0, Qt::Horizontal, tr("Col 1"));
	model.setHeaderData(1, Qt::Horizontal, tr("Col 2"));

	QTemporaryFile file; file.open();
	QDataStream stream(&file);

	ModelSerializer<> ser;
	Status stat = ser.saveHeaders(stream, &model, model.columnCount(), Qt::Horizontal);
	QVERIFY(stat.ok());
	file.flush();
	file.close();
	
	file.open();
	QStandardItemModel newModel; Status stat1;
	stat1 = ser.loadHeaders(stream, &newModel, Qt::Horizontal, stat1);

	QVERIFY(stat1.ok());
	QCOMPARE(newModel.columnCount(), model.columnCount());
	QCOMPARE(newModel.rowCount(), model.rowCount());
	for (size_t i = 0; i < model.columnCount(); ++i)
		QCOMPARE(newModel.headerData(i, Qt::Horizontal).toString(), model.headerData(i, Qt::Horizontal).toString());

	file.close();
}


void ModelSerializer_test::save_loadData()
{
	QStandardItemModel model;
	model.setHeaderData(0, Qt::Horizontal, tr("Col 1"));
	model.setHeaderData(1, Qt::Horizontal, tr("Col 2"));
	model.appendRow(QList<QStandardItem*>{new QStandardItem("0 0"), new QStandardItem("0 1")});
	model.appendRow(QList<QStandardItem*>{new QStandardItem("1 0"), new QStandardItem("1 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  0 0"), new QStandardItem("(1 0) (0 1) -> 0 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  1 0"), new QStandardItem("(1 0) (1 1) -> 0 1")});

	QTemporaryFile file; file.open();
	QDataStream stream(&file);

	ModelSerializer<> ser;
	Status stat = ser.saveData(stream, &model, QModelIndex());
	QVERIFY(stat.ok());
	file.flush();
	file.close();

	file.open();
	QStandardItemModel newModel; Status stat1;
	stat1 = ser.loadData(stream, &newModel, QModelIndex(), stat1);
	QVERIFY(stat1.ok());

	QCOMPARE(newModel.item(0, 0)->data().toString(), model.item(0, 0)->data().toString());
	QCOMPARE(newModel.item(0, 1)->data().toString(), model.item(0, 1)->data().toString());
			 
	QCOMPARE(newModel.item(1, 0)->data().toString(), model.item(1, 0)->data().toString());
	QCOMPARE(newModel.item(1, 1)->data().toString(), model.item(1, 1)->data().toString());
			 
	QCOMPARE(newModel.item(1, 0)->child(0, 0)->data().toString(), model.item(1, 0)->child(0, 0)->data().toString());
	QCOMPARE(newModel.item(1, 0)->child(0, 1)->data().toString(), model.item(1, 0)->child(0, 1)->data().toString());
	QCOMPARE(newModel.item(1, 0)->child(1, 0)->data().toString(), model.item(1, 0)->child(1, 0)->data().toString());
	QCOMPARE(newModel.item(1, 0)->child(1, 1)->data().toString(), model.item(1, 0)->child(1, 1)->data().toString());

	file.close();
}

void ModelSerializer_test::save_load()
{
	QStandardItemModel model;
	model.setHeaderData(0, Qt::Horizontal, tr("Col 1"));
	model.setHeaderData(1, Qt::Horizontal, tr("Col 2"));
	model.appendRow(QList<QStandardItem*>{new QStandardItem("0 0"), new QStandardItem("0 1")});
	model.appendRow(QList<QStandardItem*>{new QStandardItem("1 0"), new QStandardItem("1 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  0 0"), new QStandardItem("(1 0) (0 1) -> 0 1")});
	model.item(1)->appendRow(QList<QStandardItem*>{new QStandardItem("((1 0) (1 1) ->  1 0"), new QStandardItem("(1 0) (1 1) -> 0 1")});

	QTemporaryFile file;
	file.open();
	QDataStream stream(&file);

	ModelSerializer<> ser;
	Status stat = ser.save(stream, &model);
	QVERIFY(stat.ok());
	file.flush();
	file.close();

	file.open();
	QStandardItemModel newModel; Status stat1;
	stat1 = ser.load(stream, &newModel);
	QVERIFY(stat1.ok());

	QCOMPARE(newModel.columnCount(), model.columnCount());
	QCOMPARE(newModel.rowCount(), model.rowCount());
	for (size_t i = 0; i < model.columnCount(); ++i)
		QCOMPARE(newModel.headerData(i, Qt::Horizontal).toString(), model.headerData(i, Qt::Horizontal).toString());

	QCOMPARE(newModel.item(0, 0)->data().toString(), model.item(0, 0)->data().toString());
	QCOMPARE(newModel.item(0, 1)->data().toString(), model.item(0, 1)->data().toString());

	QCOMPARE(newModel.item(1, 0)->data().toString(), model.item(1, 0)->data().toString());
	QCOMPARE(newModel.item(1, 1)->data().toString(), model.item(1, 1)->data().toString());

	QCOMPARE(newModel.item(1, 0)->child(0, 0)->data().toString(), model.item(1, 0)->child(0, 0)->data().toString());
	QCOMPARE(newModel.item(1, 0)->child(0, 1)->data().toString(), model.item(1, 0)->child(0, 1)->data().toString());
	QCOMPARE(newModel.item(1, 0)->child(1, 0)->data().toString(), model.item(1, 0)->child(1, 0)->data().toString());
	QCOMPARE(newModel.item(1, 0)->child(1, 1)->data().toString(), model.item(1, 0)->child(1, 1)->data().toString());

	file.close();
}
