#include "AddDataToFolder.h"
#include <QScroller>
#include <QScreen>


AddDataToFolder::AddDataToFolder(QWidget *parent, QFileInfoModel* model)
	: QDialog(parent), model(model)
{
	ui.setupUi(this);
    treeViewInit(ui.systemTree, model);

    // ui.addressesEdit->setToolTip("Separate links with commas");

    ui.systemTree->setExpandsOnDoubleClick(true);
    connect(ui.systemTree, &QTreeView::clicked, this, &AddDataToFolder::on_treeView_clicked);
    // connect(ui.addressesEdit, &QPlainTextEdit::, this, &AddDataToFolder::finished);
    connect(ui.cancelButton, &QPushButton::clicked, this, &AddDataToFolder::finished);
    connect(ui.okButton, &QPushButton::clicked, this, &AddDataToFolder::sendIndexes);
    connect(ui.okButton, &QPushButton::clicked, this, &AddDataToFolder::finished);
}

AddDataToFolder::~AddDataToFolder()
{}

//void AddDataToFolder::addressText_edited()
//{
//    QString text = ui.addressesEdit->toPlainText();
//    QStringList list = text.split(",");
//    selected.clear();
//    ui.systemTree->blockSignals(true);
//    foreach(auto path, list)
//    {
//        QModelIndex index = model->byPath(path);
//        selected.append({ index, path });
//        ui.systemTree->setCurrentIndex(index);
//    }
//    ui.systemTree->blockSignals(false);
//}

void AddDataToFolder::sendIndexes()
{
    QModelIndexList list;
    foreach(auto elem, selected)
        list.append(elem.first);

    emit indexesSent(list);
}

void AddDataToFolder::on_treeView_clicked(const QModelIndex& index)
{
    size_t i = 0;
    foreach (auto elem, selected)
    {
        if (elem.first == index)
        {
            selected.removeAt(i);
           // updateAddressText();
            return;
        }
        i++;
    }
    QString path = model->pathFromStringList(model->getPath(index));
    selected.append({ index, path});
   // updateAddressText();
}

void AddDataToFolder::treeViewInit(QTreeView* tree, QAbstractItemModel* model1)
{
    tree->setModel(model1);

    for (size_t i = 4; i < tree->model()->columnCount(); i++)
        tree->hideColumn(i); // only 4 columns need to be displayed

    tree->setSelectionMode(QAbstractItemView::MultiSelection);
    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    const QSize availableSize = tree->screen()->availableGeometry().size();
    tree->setColumnWidth(0, tree->width() / 2);
    QScroller::grabGesture(tree, QScroller::TouchGesture);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
}

//void AddDataToFolder::updateAddressText()
//{
//    ui.addressesEdit->blockSignals(true);
//
//    ui.addressesEdit->clear();
//    foreach(auto elem, selected)
//    {
//        QString text = elem.second + (selected.last().second == elem.second ? "" : ", ");
//        ui.addressesEdit->appendPlainText(text);
//    }
//    ui.addressesEdit->blockSignals(false);
//}
//
//void AddDataToFolder::enterPressEvent(QKeyEvent* event) {
//    if (event->key() == Qt::Key_Return) {
//        addressText_edited();
//    }
//    else {
//        keyPressEvent(event);
//    }
//}

