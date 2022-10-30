#include "OfflineFileManager.h"
#include <QCommandLineParser>
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QScreen>
#include <Qscroller>
#include <QStringList>
#include <QThread>

const QString savingFile = "tmp.bin";


OfflineFileManager::OfflineFileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    model = new QFileInfoModel();
    //model->writeFile("", maxDepth);

    model->readFile(savingFile);

    treeViewInit(ui.fileSystemTree, model);

    connect(ui.actionClose, &QAction::triggered, this, &OfflineFileManager::close);
    connect(ui.updateButton, &QToolButton::clicked, this, &OfflineFileManager::on_updateButton_clicked);
    connect(ui.fileSystemTree, &QTreeView::activated, this, &OfflineFileManager::on_treeWidget_clicked);
}

OfflineFileManager::~OfflineFileManager()
{
    model->writeFile(savingFile, maxDepth);
    delete model;
}

void OfflineFileManager::on_treeWidget_clicked(QModelIndex index)
{
    QList<QString> path;
    while (index.isValid())
    {
        path.append(index.data().toString());
        index = index.parent();
    }
    std::reverse(path.begin(), path.end());

    QString spath;
    foreach(auto file, path)
        spath +=  '/' + file;

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
