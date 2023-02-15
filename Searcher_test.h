#pragma once

#include <QObject>
#include <QTest>

#include "Searcher.h"
#include "QFileInfoModel.h"

#include <chrono>

constexpr std::chrono::milliseconds kTimeShift(3242);
const QString kTestPath("");

class Searcher_test : public QObject
{
	Q_OBJECT

private slots:
	void search_by_name();
	void search_by_type();
	void search_by_hash();
	void search_by_minimum_creation_time();
	void search_by_maximum_creationand_minimum_edited_time();

private:

	static constexpr size_t kDepth = 2;
	static constexpr size_t kMinStep = 3, kMaxStep = 1000;
	static constexpr size_t kTestCount = 10;
};

