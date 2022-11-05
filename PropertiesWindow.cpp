#include "PropertiesWindow.h"

#include <QHeaderView>

PropertiesWindow::PropertiesWindow(QWidget* parent, QModelIndex index, QFileInfoModel* model)
    : QTableWidget(parent), index(index)
{
    properties = new PropertiesLogic(index, model);

    //this->setFixedHeight(parent->height());
    this->setFixedWidth(parent->width());
    this->setColumnCount(2);
    this->setRowCount(columnsNumber);
    this->setColumnWidth(0, 2 * this->width() / 6);
    this->setColumnWidth(1, 3 * this->width() / 5);

    //for (size_t i = 0; i < this->rowCount(); i++)
    //    this->setRowHeight(i, this->height() / 70);

    this->setRowHeight(columnsNumber, this->height() / 8);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setCursor(QCursor(Qt::ArrowCursor));

    this->setItem((int)PropertiesOrder::NAME, 0, new QTableWidgetItem("Name:"));
    this->setItem((int)PropertiesOrder::NAME, 1, new QTableWidgetItem(properties->getName()));

    this->setItem((int)PropertiesOrder::FULL_PATH, 0, new QTableWidgetItem("Full path:"));
    this->setItem((int)PropertiesOrder::FULL_PATH, 1, new QTableWidgetItem(properties->getFullPath()));

    this->setItem((int)PropertiesOrder::TYPE, 0, new QTableWidgetItem("Type:"));
    this->setItem((int)PropertiesOrder::TYPE, 1, new QTableWidgetItem(properties->getType()));

    this->setItem((int)PropertiesOrder::SIZE, 0, new QTableWidgetItem("Size:"));
    this->setItem((int)PropertiesOrder::SIZE, 1, new QTableWidgetItem(properties->getSize()));

    this->setItem((int)PropertiesOrder::ICON_NAME, 0, new QTableWidgetItem("Identical count:"));
    this->setItem((int)PropertiesOrder::ICON_NAME, 1, new QTableWidgetItem(properties->getIndeticalCopiesNumber()));

    this->setItem((int)PropertiesOrder::GROUP, 0, new QTableWidgetItem("Group:"));
    this->setItem((int)PropertiesOrder::GROUP, 1, new QTableWidgetItem(properties->getGroup()));

    this->setItem((int)PropertiesOrder::OWNER, 0, new QTableWidgetItem("Owner:"));
    this->setItem((int)PropertiesOrder::OWNER, 1, new QTableWidgetItem(properties->getOwner()));

    this->setItem((int)PropertiesOrder::OWNER_ID, 0, new QTableWidgetItem("Owner ID:"));
    this->setItem((int)PropertiesOrder::OWNER_ID, 1, new QTableWidgetItem(properties->getOwnerid()));

    this->setItem((int)PropertiesOrder::DATE_CREATED, 0, new QTableWidgetItem("Date created:"));
    this->setItem((int)PropertiesOrder::DATE_CREATED, 1, new QTableWidgetItem(properties->getCreated()));

    this->setItem((int)PropertiesOrder::DATE_MODIDFIED, 0, new QTableWidgetItem("Date modified:"));
    this->setItem((int)PropertiesOrder::DATE_MODIDFIED, 1, new QTableWidgetItem(properties->getLastModified()));

    this->setItem((int)PropertiesOrder::MD5, 0, new QTableWidgetItem("MD5 hash:"));
    this->setItem((int)PropertiesOrder::MD5, 1, new QTableWidgetItem(properties->getMd5()));

    this->setItem((int)PropertiesOrder::CUSTOM_METHADATA, 0, new QTableWidgetItem("Custom metadata:"));
    this->setItem((int)PropertiesOrder::CUSTOM_METHADATA, 1, new QTableWidgetItem(properties->getCustomMethadata()));

    this->setHorizontalHeaderItem(0, new QTableWidgetItem("Property"));
    this->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
    this->verticalHeader()->hide();


    QTableWidgetItem* customItem = this->item(this->rowCount() - 1,
        this->columnCount() - 1);
    Qt::ItemFlags writeFlag = customItem->flags();

    // Make readonly
    for (size_t i = 0; i < this->rowCount(); ++i)
        for (size_t j = 0; j < this->columnCount(); ++j) {
            QTableWidgetItem* item = this->item(i, j);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }

    // Set custom meta editable
    customItem->setFlags(writeFlag);
}

void PropertiesWindow::saveTextSlot()
{
    emit saveTextSignal(this->item(rowCount() - 1, 
        columnCount() - 1)->data(0).toString(), this->index);
}
