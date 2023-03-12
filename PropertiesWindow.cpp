#include "PropertiesWindow.h"

#include <QHeaderView>

namespace manager {

PropertiesWindow::PropertiesWindow(PropertiesLogic* logic, QWidget* parent)
    : QDialog(parent), logic(logic)
{
    layout.setParent(this);
    this->setWindowTitle("Properties");
    this->setFixedHeight(550);
    this->setFixedWidth(400);

    table.setParent(this);

    buttonOk.setParent(this);
    buttonCancel.setParent(this);
    buttonSave.setParent(this);
    buttonCancel.setText("Cancel");
    buttonOk.setText("OK");
    buttonSave.setText("Save");

    connect(&buttonCancel, &QPushButton::clicked, this, &QWidget::close);
    connect(&buttonOk, &QPushButton::clicked, this, &QWidget::close);
    connect(&buttonOk, &QPushButton::clicked, this, &PropertiesWindow::save);
    connect(&buttonSave, &QPushButton::clicked, this, &PropertiesWindow::save);

    layout.addWidget(&table, 0);
    layout.addWidget(&buttonCancel);
    layout.addWidget(&buttonSave);
    layout.addWidget(&buttonOk);
    this->setLayout(&layout);

    table.setFixedWidth(parent->width());
    table.setColumnCount(2);
    table.setRowCount(rowsNumber);
    table.setColumnWidth(0, 2 * this->width() / 6);
    table.setColumnWidth(1, 3 * this->width() / 5);

    table.setRowHeight(rowsNumber, table.height() / 8);
    table.setAttribute(Qt::WA_DeleteOnClose);
    table.setCursor(QCursor(Qt::ArrowCursor));

    table.setItem(NAME, 0, new QTableWidgetItem("Name:"));
    table.setItem(NAME, 1, new QTableWidgetItem(logic->getName()));

    table.setItem(FULL_PATH, 0, new QTableWidgetItem("Full path:"));
    table.setItem(FULL_PATH, 1, new QTableWidgetItem(logic->getFullPath()));

    table.setItem(TYPE, 0, new QTableWidgetItem("Type:"));
    table.setItem(TYPE, 1, new QTableWidgetItem(logic->getType()));

    table.setItem(SIZE, 0, new QTableWidgetItem("Size:"));
    table.setItem(SIZE, 1, new QTableWidgetItem(logic->getSize()));

    table.setItem(ICON_NAME, 0, new QTableWidgetItem("Identical count:"));
    table.setItem(ICON_NAME, 1, new QTableWidgetItem(logic->getIndeticalCopiesNumber()));

    table.setItem(GROUP, 0, new QTableWidgetItem("Group:"));
    table.setItem(GROUP, 1, new QTableWidgetItem(logic->getGroup()));

    table.setItem(OWNER, 0, new QTableWidgetItem("Owner:"));
    table.setItem(OWNER, 1, new QTableWidgetItem(logic->getOwner()));

    table.setItem(OWNER_ID, 0, new QTableWidgetItem("Owner ID:"));
    table.setItem(OWNER_ID, 1, new QTableWidgetItem(logic->getOwnerid()));

    table.setItem(DATE_CREATED, 0, new QTableWidgetItem("Date created:"));
    table.setItem(DATE_CREATED, 1, new QTableWidgetItem(logic->getCreated()));

    table.setItem(DATE_MODIDFIED, 0, new QTableWidgetItem("Date modified:"));
    table.setItem(DATE_MODIDFIED, 1, new QTableWidgetItem(logic->getLastModified()));

    table.setItem(HASH, 0, new QTableWidgetItem("Hash:"));
    table.setItem(HASH, 1, new QTableWidgetItem(logic->getHash()));

    table.setItem(CUSTOM_METHADATA, 0, new QTableWidgetItem("Custom metadata:"));
    table.setItem(CUSTOM_METHADATA, 1, new QTableWidgetItem(logic->getCustomMethadata()));

    table.setHorizontalHeaderItem(0, new QTableWidgetItem("Property"));
    table.setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
    table.verticalHeader()->hide();


    QTableWidgetItem* customItem = table.item(table.rowCount() - 1,
        table.columnCount() - 1);
    Qt::ItemFlags writeFlag = customItem->flags();

    // Make readonly
    for (size_t i = 0; i < table.rowCount(); ++i)
        for (size_t j = 0; j < table.columnCount(); ++j) {
            QTableWidgetItem* item = table.item(i, j);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }

    // Set custom meta editable
    customItem->setFlags(writeFlag);
}

void PropertiesWindow::save()
{
    logic->setCustomMethadata(table.item(table.rowCount() - 1, table.columnCount() - 1)->data(0).toString());
    logic->saveMeta();
}

}
