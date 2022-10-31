#include "PropertiesWindow.h"

PropertiesWindow::PropertiesWindow(QWidget* parent)
    : QTableWidget(parent)
{
}

void PropertiesWindow::saveTextSlot()
{
    emit saveTextSignal(this->item(rowCount() - 1, 
        columnCount() - 1)->data(0).toString());
}
