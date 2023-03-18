#pragma once

#include <QDialog>
#include <QDateTime>
#include <QFileIconProvider>

#include "ui_SearchWindow.h"


namespace manager {

class SearchController;

class SearchWindow : public QDialog
{
	Q_OBJECT

public:
    SearchWindow(QWidget* parent = nullptr);
	~SearchWindow();

	inline void setController(SearchController* controller){
		this->controller = controller;
	}

	inline QString getNameLine() {
		return ui.nameLine->text();
	}
	inline QString getTypeLine() {
		return ui.typeLine->text();
	}
	inline QString getHashLine() {
		return ui.hashLine->text();
	}
	inline QString getMetadataLine() {
		return ui.metadataLine->text();
	}
	inline QDateTime getMinCreatedTime() {
		return ui.minCreatedTime->dateTime();
	}
	inline QDateTime getMaxCreatedTime() {
		return ui.maxCreatedTime->dateTime();
	}
	inline QDateTime getMinEditedTime() {
		return ui.minEditedTime->dateTime();
	}
	inline QDateTime getMaxEditedTime() {
		return ui.maxEditedTime->dateTime();
	}
	inline QString getPath() {
		return ui.pathLine->text();
	}

	// TODO: Create getters for Check boxes

	void insertRow(QList<QTableWidgetItem*> data, size_t row);

signals:
	void onCustomContextMenu(const QPoint&);

public slots:
	void setPath(const QString& path) {
		ui.pathLine->setText(path);
	};

private slots:
    void search();

private:
	Ui::SearchWindowClass ui;
	QFileIconProvider provider;

	SearchController* controller;

	void initTable();
};

}
