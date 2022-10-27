#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineFileManager.h"

// #include "qjsonmodel.h"


class OfflineFileManager : public QMainWindow
{
    Q_OBJECT

public:
    OfflineFileManager(QWidget *parent = nullptr);
    ~OfflineFileManager();

private:
    void on_treeView_clicked(const QModelIndex& index);
    void on_updateButton_clicked();

    void makeOffline(); // disconnect from live updating

    Ui::OfflineFileManagerClass ui;
    QAbstractItemModel* model;
};
