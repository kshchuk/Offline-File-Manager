#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineFileManager.h"
#include <QMessageBox>
#include <QTreeView>
#include <QThread>
#include <QList>

#include "QFileInfoModel.h"



class OfflineFileManager : public QMainWindow
{
    Q_OBJECT

        enum Regime { FILESYSTEM, EXTERNAL_DRIVES };

public:
    OfflineFileManager(QWidget* parent = nullptr);
    ~OfflineFileManager();

private slots:
    void on_treeWidget_clicked(QModelIndex index);
    void action_openInFileExplorer();
    void action_openFile();
    void action_Properties();
    void action_addDataToVirtualFolder();
    void on_customContextMenu(const QPoint& point);
    void on_updateButton_clicked();
    void on_saveAction_triggered();
    void on_openAction_triggered();
    void on_homeButton_clicked();
    void on_editLine_editingFinished();
    void on_upButton_clicked();
    void on_addFolderButton_clicked();
    void saveMeta(const QString&, const QModelIndex&);
    void editFileName(const QModelIndex& index);
    void editFileNameA();
    void addDataToVirtualFolder(QModelIndexList list);
    void removeElement();
    void errorString(QString);

private:
    Ui::OfflineFileManagerClass ui;
    QFileInfoModel* model = nullptr;
    Regime regime = EXTERNAL_DRIVES;
    size_t maxDepth = 10;
    QMessageBox saveMessage;

    QString error;
    void treeViewInit(QTreeView* tree, QFileInfoModel* model);
};
