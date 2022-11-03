#include "PropertiesWindow.h"

PropertiesWindow::PropertiesWindow(QWidget* parent, QModelIndex index)
    : QTableWidget(parent), index(index)
{
}

void PropertiesWindow::saveTextSlot()
{
    emit saveTextSignal(this->item(rowCount() - 1, 
        columnCount() - 1)->data(0).toString(), this->index);
}
