#include "OfflineFileManager.h"
#include <QCommandLineParser>
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>
#include <Qscroller>
#include <QStringList>
#include <QThread>

#include <qexception.h>

const QString savingFile = "static.fsh";


OfflineFileManager::OfflineFileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    model = new QFileInfoModel();
    //model->writeFile("", maxDepth);
    
    try {
        model->readFile(savingFile);
    }
    catch (...) {};

    treeViewInit(ui.fileSystemTree, model);

    connect(ui.actionClose, &QAction::triggered, this, &OfflineFileManager::close);
    connect(ui.updateButton, &QToolButton::clicked, this, &OfflineFileManager::on_updateButton_clicked);
    connect(ui.fileSystemTree, &QTreeView::activated, this, &OfflineFileManager::on_treeWidget_clicked);
    connect(ui.actionSave, &QAction::triggered, this, &OfflineFileManager::on_saveAction_triggered);
    connect(ui.actionOpen, &QAction::triggered, this, &OfflineFileManager::on_openAction_triggered);
}

OfflineFileManager::~OfflineFileManager()
{
    model->writeFile(savingFile, maxDepth);
    delete model;
}

void OfflineFileManager::on_treeWidget_clicked(QModelIndex index)
{
    QList<QString> path = model->getPath(index);
    QString spath;
    foreach(auto file, path) spath += '/' + file;
    spath.remove(0, 1);

    ui.addressLine->setText(spath);
}

void OfflineFileManager::on_updateButton_clicked()
{
    switch (regime)
    {
    case OfflineFileManager::FILESYSTEM:
    {
        treeViewInit(ui.fileSystemTree, new QFileSystemModel);

        this->model->genStaticSystemModel(maxDepth);
        treeViewInit(ui.fileSystemTree, model);
        break;
    }
    case OfflineFileManager::EXTERNAL_DRIVES:
    {
        this->model->genExternalDrivesModel(maxDepth);
        // treeViewInit(ui.fileSystemTree, model->readFile("tmp.bin"));
        treeViewInit(ui.fileSystemTree, model);
        break;
    }
    default:
        break;
    }
}

void OfflineFileManager::on_saveAction_triggered() 
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        ("Save File"),"", ("File system hierarchy (*.fsh)"));

    try {
        model->writeFile(fileName, maxDepth);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Offline File Manager"),
            tr(e.what()), QMessageBox::Close);
    }
}

void OfflineFileManager::on_openAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
        "", ("File system hierarchy (*.fsh)"));
    
    try {
        model->readFile(fileName);
    }
    catch(const std::exception &e) {
        QMessageBox::critical(this, tr("Offline File Manager"),
            tr(e.what()), QMessageBox::Close);
    }
    treeViewInit(ui.fileSystemTree, model);
}

void OfflineFileManager::treeViewInit(QTreeView* tree, QAbstractItemModel* model)
{
    tree->setModel(model);

    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    const QSize availableSize = tree->screen()->availableGeometry().size();
    tree->setColumnWidth(0, tree->width() / 2);
    QScroller::grabGesture(tree, QScroller::TouchGesture);

    connect(tree, &QTreeView::clicked, this, &OfflineFileManager::on_treeWidget_clicked);
}
