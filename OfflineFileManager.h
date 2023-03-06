#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineFileManager.h"
#include <QMessageBox>
#include <QTreeView>
#include <QThread>
#include <QList>

#include "QFileInfoModel.h"


class OfflineFileManagerController
{
   // Q_OBJECT

    // enum class Regime { FILESYSTEM, EXTERNAL_DRIVES, GOOGLE_DRIVE };

public:
    
};

namespace view
{

    //class OfflineFileManagerView : public QMainWindow
    //{
    //    //Q_OBJECT

    //public:
    //    explicit OfflineFileManagerView(QWidget* parent = nullptr);

    //    void setModel(model::QFileInfoModel* model);

    //private slots:



    //private:
    //    model::QFileInfoModel* model = nullptr;
    //    Ui::OfflineFileManagerClass ui;
    //};


    class OfflineFileManager : public QMainWindow
    {
        Q_OBJECT



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
        void addDataToVirtualFolder(QModelIndexList indexes, QStringList paths);
        void removeElement();
        void errorString(QString);
        void setExternalDrivesregime();
        void setAllDrivesregime();
        void setGoogleDriveRegime();
        void setMaxDepth();
        void setTempValue(int i);
        void setMaxDepthFromTempValue();
        void highlightElem(QModelIndex index);
        void search();
        void ConnectGoogleDrive();
        void robocopyOpen();
        void treeViewInit();


    private:
        QFileInfoModel* model = nullptr;
        Regime regime = EXTERNAL_DRIVES;
        size_t maxDepth = 100;
        QMessageBox saveMessage;

        int tempValue;

        QString error;

        void runProgressBar();
    };

}