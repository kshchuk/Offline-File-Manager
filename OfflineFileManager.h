#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineFileManager.h"
#include <QFileSystemModel>

class OfflineFileManager : public QMainWindow
{
    Q_OBJECT

public:
    OfflineFileManager(QWidget *parent = nullptr);
    ~OfflineFileManager();

private slots:
    void set_AddresLineText(const QModelIndex&);

private:
    Ui::OfflineFileManagerClass ui;
    QFileSystemModel model;
};
