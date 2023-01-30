#pragma once
#include <QObject>
#include <QTest>
#include <QStandardItemModel>

#include "ModelSerializer.h"


class ModelSerializer_test : public QObject
{
	Q_OBJECT

private slots:
	void save();
	void load();
};

