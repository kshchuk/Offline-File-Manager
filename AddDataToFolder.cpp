#include "AddDataToFolder.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QScroller>
#include <QScreen>
#include <QMenu>


AddDataToFolder::AddDataToFolder(QWidget *parent, QFileInfoModel* model)
	: QDialog(parent), model(model)
{
	ui.setupUi(this);
    treeViewInit(ui.systemTree, model);

    ui.systemTree->setExpandsOnDoubleClick(true);
    connect(ui.systemTree, &QTreeView::clicked, this, &AddDataToFolder::on_treeView_clicked);
    connect(ui.cancelButton, &QPushButton::clicked, this, &AddDataToFolder::finished);
    connect(ui.okButton, &QPushButton::clicked, this, &AddDataToFolder::sendInfo);
    connect(ui.okButton, &QPushButton::clicked, this, &AddDataToFolder::finished);
    connect(ui.clearButton, &QPushButton::clicked, this, &AddDataToFolder::clearAll);
    connect(ui.fromExplorerButton, &QPushButton::clicked, this, &AddDataToFolder::selectFromExplorer);
    connect(ui.fileList, &QListWidget::customContextMenuRequested, this, &AddDataToFolder::on_customContextMenu);

    ui.fileList->setContextMenuPolicy(Qt::CustomContextMenu);
}

AddDataToFolder::~AddDataToFolder()
{}

void AddDataToFolder::selectFromExplorer()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
        selectedExternal += dialog.selectedFiles();
    updateSelectedFromExplorer();
}

void AddDataToFolder::sendInfo()
{
    QModelIndexList list;
    foreach(auto elem, selected)
        list.append(elem.first);

    emit infoSent(list, selectedExternal);
}

void AddDataToFolder::clearAll()
{
    selected.clear();
    selectedExternal.clear();
    ui.systemTree->clearSelection();
}

void AddDataToFolder::removeFromList()
{
    QModelIndex i = ui.fileList->currentIndex();

    for (size_t j = 0; j < selectedExternal.size();j++)
        if (!i.data().toString().compare(selectedExternal[j], Qt::CaseInsensitive)) {
            selectedExternal.remove(j);
            break;
        }
    delete ui.fileList->itemFromIndex(i);
}

void AddDataToFolder::on_customContextMenu(const QPoint& point)
{
    QPoint globalPos = ui.fileList->mapToGlobal(point);
    QMenu menu(this);
    menu.addAction("Remove",
        this, &AddDataToFolder::removeFromList);
    menu.exec(globalPos);
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

    for (size_t i = 2; i < tree->model()->columnCount(); i++)
        tree->hideColumn(i); // only 4 columns need to be displayed

    tree->setSelectionMode(QAbstractItemView::MultiSelection);
    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    const QSize availableSize = tree->screen()->availableGeometry().size();
    tree->setColumnWidth(0, 2*tree->width() / 3);
    QScroller::grabGesture(tree, QScroller::TouchGesture);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
}

void AddDataToFolder::updateSelectedFromExplorer()
{

    ui.fileList->clear();
    foreach(auto elem, selectedExternal) {
        QFileInfo info(elem);
        QListWidgetItem* item = new QListWidgetItem(icons.icon(info), info.fileName());
        ui.fileList->addItem(item);
    }
}

