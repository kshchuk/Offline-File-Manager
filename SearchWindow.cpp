#include "SearchWindow.h"

#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>

#include "SearchController.h"

namespace manager {

SearchWindow::SearchWindow(QWidget* parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize({ 661, 412 });
	initTable();
	ui.resultsTable->verticalHeader()->hide();

	connect(ui.timeCreatedMinCheckBox, &QCheckBox::clicked, ui.minCreatedTime, &QDateTimeEdit::setEnabled);
	connect(ui.timeCreatedMaxCheckBox, &QCheckBox::clicked, ui.maxCreatedTime, &QDateTimeEdit::setEnabled);
	connect(ui.timeEditedMinCheckBox, &QCheckBox::clicked, ui.minEditedTime, &QDateTimeEdit::setEnabled);
	connect(ui.timeEditedMaxCheckBox, &QCheckBox::clicked, ui.maxEditedTime, &QDateTimeEdit::setEnabled);

	connect(ui.searchButton, &QPushButton::clicked, this, &SearchWindow::search);

	connect(ui.resultsTable, &QTableWidget::customContextMenuRequested, this, &SearchWindow::onCustomContextMenu);
}

SearchWindow::~SearchWindow()
{
	for (size_t i = 0; i < ui.resultsTable->rowCount(); ++i) {
		for (size_t j = 0; j < 4; ++j)
			delete ui.resultsTable->item(i, j);
	}
}

void SearchWindow::insertRow(QList<QTableWidgetItem*> data, size_t row)
{
	ui.resultsTable->insertRow(row);

	ui.resultsTable->setItem(row, 0, data[0]);
	ui.resultsTable->setItem(row, 1, data[1]);
	ui.resultsTable->setItem(row, 2, data[2]);
	ui.resultsTable->setItem(row, 3, data[3]);
}

/*void SearchWindow::onCustomContextMenu(const QPoint& point)
{
	QPoint globalPos = ui.resultsTable->mapToGlobal(point);
	QMenu menu(this);
	menu.addAction("Open",
		this, &SearchWindow::open);
	menu.exec(globalPos);
}*/

void SearchWindow::initTable()
{
	ui.resultsTable->clear();
	ui.resultsTable->setFixedWidth(this->width());

	ui.resultsTable->setColumnCount(4);
	ui.resultsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
	ui.resultsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Path"));
	ui.resultsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Type"));
	ui.resultsTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Creation time"));

	ui.resultsTable->setColumnWidth(0, 2 * ui.resultsTable->width() / 12);
	ui.resultsTable->setColumnWidth(1, 5 * ui.resultsTable->width() / 12);
	ui.resultsTable->setColumnWidth(2, 2 * ui.resultsTable->width() / 12);
	ui.resultsTable->setColumnWidth(3, 3 * ui.resultsTable->width() / 12);

	ui.resultsTable->setContextMenuPolicy(Qt::CustomContextMenu);
}

void SearchWindow::search(){
    controller->search();
};

}
