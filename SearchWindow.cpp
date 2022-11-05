#include "SearchWindow.h"

#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>


SearchWindow::SearchWindow(QFileInfoModel* model, QString path, QWidget *parent)
	: QDialog(parent), path(path), model(model)
{
	ui.setupUi(this);

	searcher = new Searcher(model);

	ui.pathLine->setText(path);

	initTable();

	ui.resultsTable->verticalHeader()->hide();

	connect(ui.timeCreatedMinCheckBox, &QCheckBox::clicked, ui.minCreatedTime, &QDateTimeEdit::setEnabled);
	connect(ui.timeCreatedMaxCheckBox, &QCheckBox::clicked, ui.maxCreatedTime, &QDateTimeEdit::setEnabled);
	connect(ui.timeEditedMinCheckBox, &QCheckBox::clicked, ui.minEditedTime, &QDateTimeEdit::setEnabled);
	connect(ui.timeEditedMaxCheckBox, &QCheckBox::clicked, ui.maxEditedTime, &QDateTimeEdit::setEnabled);

	connect(ui.searchButton, &QPushButton::clicked, this, &SearchWindow::search);
	connect(searcher, &Searcher::found, this, &SearchWindow::foundSlot);

	connect(ui.resultsTable, &QTableWidget::customContextMenuRequested, this, &SearchWindow::on_customContextMenu);
}

SearchWindow::~SearchWindow()
{
	for (size_t i = 0; i < ui.resultsTable->rowCount(); ++i) {
		for (size_t j = 0; j < 4; ++j)
			delete ui.resultsTable->item(i, j);
	}
}

void SearchWindow::foundSlot(QModelIndex index)
{
	foundIndexes.append(index);

	size_t row = foundIndexes.size() - 1;
	ui.resultsTable->insertRow(row);
	
	QFileInfo info(index.data().toString());
	ui.resultsTable->setItem(row, 0, new QTableWidgetItem(provider.icon(info), info.fileName()));
	ui.resultsTable->setItem(row, 1, new QTableWidgetItem(index.siblingAtColumn((int)ColunmsOrder::FULL_PATH).data().toString()));
	ui.resultsTable->setItem(row, 2, new QTableWidgetItem(index.siblingAtColumn((int)ColunmsOrder::TYPE).data().toString()));
	ui.resultsTable->setItem(row, 3, new QTableWidgetItem(index.siblingAtColumn((int)ColunmsOrder::DATE_CREATED).data().toString()));

}

void SearchWindow::open()
{
	qulonglong row =  ui.resultsTable->currentIndex().row();
	QModelIndex index = foundIndexes[row];
	emit infoSent(index);
}

void SearchWindow::on_customContextMenu(const QPoint& point)
{
	QPoint globalPos = ui.resultsTable->mapToGlobal(point);
	QMenu menu(this);
	menu.addAction("Open",
		this, &SearchWindow::open);
	menu.exec(globalPos);
}

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

void SearchWindow::search()
{
	foundIndexes.clear();
	if (model->rowCount() > 0)
	{

	initTable();
	ui.searchButton->blockSignals(true);
	
	QVariant* criterias = new QVariant[searcher->criteriasCount + 1];
	
	if (!ui.nameLine->text().isEmpty())
		criterias[searcher->NAME] = ui.nameLine->text();
	if (!ui.typeLine->text().isEmpty())
		criterias[searcher->TYPE] = ui.typeLine->text();

	if (!ui.hashLine->text().isEmpty())
		criterias[searcher->HASH] = ui.hashLine->text();
	if (!ui.metadataLine->text().isEmpty())
		criterias[searcher->CUSTOM_META] = ui.metadataLine->text();

	if (ui.timeCreatedMinCheckBox->isChecked())
		criterias[searcher->CREATION_TIME_MIN] = ui.minCreatedTime->dateTime();
	if (ui.timeCreatedMaxCheckBox->isChecked())
		criterias[searcher->CREATION_TIME_MAX] = ui.maxCreatedTime->dateTime();

	if (ui.timeEditedMinCheckBox->isChecked())
		criterias[searcher->EDIT_TIME_MIN] = ui.minEditedTime->dateTime();
	if (ui.timeEditedMaxCheckBox->isChecked())
		criterias[searcher->EDIT_TIME_MAX] = ui.maxEditedTime->dateTime();

	searcher->search(criterias, ui.pathLine->text());

	ui.searchButton->blockSignals(false);
	}
}

