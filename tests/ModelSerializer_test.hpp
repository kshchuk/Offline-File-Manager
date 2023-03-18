#pragma once

#include <QTest>
#include "ModelSerializer.h"

class ModelSerializer_test : public QObject {
    Q_OBJECT

private slots:
    void testSaveLoad() {
        // Create a model to serialize
        QStandardItemModel model;
        model.setHorizontalHeaderLabels(QStringList{ "Name", "Age" });
        auto john = new QStandardItem("John");
        john->setChild(0, 0, new QStandardItem("Alice"));
        john->setChild(0, 1, new QStandardItem("25"));
        john->setChild(1, 0, new QStandardItem("Bob"));
        john->setChild(1, 1, new QStandardItem("30"));
        auto mary = new QStandardItem("Mary");
        mary->setChild(0, 0, new QStandardItem("Charlie"));
        mary->setChild(0, 1, new QStandardItem("20"));
        mary->setChild(1, 0, new QStandardItem("David"));
        mary->setChild(1, 1, new QStandardItem("35"));
        model.setItem(0, 0, john);
        model.setItem(1, 0, mary);

        // Create a serializer and serialize the model to a QByteArray
        manager::ModelSerializer<> serializer;
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        QVERIFY(serializer.save(stream, &model).isOk());

        // Deserialize the model from the QByteArray
        QStandardItemModel loadedModel;
        QDataStream loadStream(&data, QIODevice::ReadOnly);
        QVERIFY(serializer.load(loadStream, &loadedModel).isOk());

        // Compare the original and loaded models
        QCOMPARE(loadedModel.rowCount(), model.rowCount());
        QCOMPARE(loadedModel.columnCount(), model.columnCount());
        for (int i = 0; i < model.rowCount(); ++i) {
            auto originalItem = model.item(i, 0);
            auto loadedItem = loadedModel.item(i, 0);
            QCOMPARE(loadedItem->text(), originalItem->text());
            QCOMPARE(loadedItem->rowCount(), originalItem->rowCount());
            QCOMPARE(loadedItem->columnCount(), originalItem->columnCount());
            for (int j = 0; j < originalItem->rowCount(); ++j) {
                for (int k = 0; k < originalItem->columnCount(); ++k) {
                    auto originalChild = originalItem->child(j, k);
                    auto loadedChild = loadedItem->child(j, k);
                    QCOMPARE(loadedChild->text(), originalChild->text());
                }
            }
        }
    }
};