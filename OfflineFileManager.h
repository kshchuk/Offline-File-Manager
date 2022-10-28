#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineFileManager.h"
#include <QFileSystemModel>
#include <QTreeView>

// #include "qjsonmodel.h"


class OfflineFileManager : public QMainWindow
{
    Q_OBJECT

    enum Regime { FILESYSTEM, EXTERNAL_DRIVES };

public:
    OfflineFileManager(QWidget *parent = nullptr);
    ~OfflineFileManager();

private slots:
    void on_treeWidget_clicked(QModelIndex index);
    void on_updateButton_clicked();

private:
    Ui::OfflineFileManagerClass ui;
    QAbstractItemModel* model = nullptr;
    Regime regime = EXTERNAL_DRIVES;
    size_t maxDepth = 20;


   void treeViewInit(QTreeView* tree, QAbstractItemModel* model);
};
