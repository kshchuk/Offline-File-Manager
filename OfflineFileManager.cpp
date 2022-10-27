#include "OfflineFileManager.h"
#include <QCommandLineParser>
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QScreen>
#include <Qscroller>
#include <QStringList>

#include "QJsonModel.h"
#include "QModelLoader.h"


OfflineFileManager::OfflineFileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QModelLoader loader(model);
    loader.makeFileSystemModel();
    model = loader.getModel();

    ui.fileSystemModelView->setModel(model);

    ui.fileSystemModelView->setAnimated(false);
    ui.fileSystemModelView->setIndentation(20);
    ui.fileSystemModelView->setSortingEnabled(true);
    const QSize availableSize = ui.fileSystemModelView->screen()->availableGeometry().size();
    ui.fileSystemModelView->setColumnWidth(0, ui.fileSystemModelView->width() / 2);
    QScroller::grabGesture(ui.fileSystemModelView, QScroller::TouchGesture);

    connect(ui.fileSystemModelView, &QTreeView::clicked, this, &OfflineFileManager::on_treeView_clicked);
    connect(ui.actionClose, &QAction::triggered, this, &OfflineFileManager::close);
    connect(ui.updateButton, &QToolButton::clicked, this, &OfflineFileManager::on_updateButton_clicked);

    connect(dynamic_cast<QFileSystemModel*> (model), &QFileSystemModel::directoryLoaded, 
        this, &OfflineFileManager::makeOffline);
}

OfflineFileManager::~OfflineFileManager()
{
    delete model;
}

void OfflineFileManager::on_treeView_clicked(const QModelIndex & index)
{
    //QFileSystemModel* fmodel = dynamic_cast<QFileSystemModel*> (model);
    //if (fmodel != nullptr);
    //{
    //    QString path = fmodel->filePath(index);
    //    ui.addressLine->setText(path);
    //}
}

void OfflineFileManager::on_updateButton_clicked()
{
    //QModelLoader loader(&model);
    //loader.makeFileSystemModel();
    
    // this->makeOffline();
}

void OfflineFileManager::makeOffline()
{
    //QModelLoader loader(&this->model);
    //loader.writeModel("tmp.json");

    //QStringList headers;
    //for (size_t i = 0; i < 4 /*sections count*/; i++)
    //    headers.append(model->headerData(i, Qt::Horizontal).toString());

    //loader.readModel("tmp.json", headers);
}
