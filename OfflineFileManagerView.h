#pragma once

#include <QtWidgets/QMainWindow>

#include <QMessageBox>
#include <QMenu>
#include <QList>

#include "ui_OfflineFileManager.h"

#include "QFileInfoModel.h"


namespace manager
{
    class OfflineFileManagerViewActions : public QObject
    {
        Q_OBJECT

   public:
    signals:
        void actionSaveToFile();
        void actionLoadFromFile();
        void actionUpdate();
        void actionGoUpper();
        void actionGoHome();
        void actionAddFolder();
        void actionSearch();
        void actionOpenRobocopy();

        void actionExternalDrivesRegime();
        void actionAllDrivesRegime();
        void actionGoogleDriveRegime();

        void actionSetMaxReadingDepth();
        
    };

    class OfflineFileManagerViewButtons : public QObject
    {
        Q_OBJECT

    public:
     signals:
        void onUpdateButtonClicked();
        void onHomeButtonClicked();
        void onUpButtonClicked();
        void onAddFolderButtonClicked();
        void onSearchButtonClicked();
    };


    class OfflineFileManagerViewTree : public QObject
    {
        Q_OBJECT

    public:
     signals:
        void onTreeWidgetClicked(const QModelIndex &);
        void onTreeWidgetDoubleClicked(const QModelIndex &);
        void onCustomContextMenuRequested(const QPoint &point);
    };

    class OfflineFileManagerView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit OfflineFileManagerView(QWidget *parent = nullptr);

        inline void setModel(QFileInfoModel *model){
            this->model = model;
            ui.fileSystemTree->setModel(model);
            this->update();
        };
        inline QModelIndex getCurrentIndex() {
            
            return ui.fileSystemTree->currentIndex();
        };
        inline QPoint getGlobalPos(const QPoint& point) {
            return this->ui.fileSystemTree->mapToGlobal(point);
        }
        inline QString getEditLineText() {
            return this->ui.addressLine->text();
        };

    signals:
        void onEditLineEditingFinished();

    public slots:
        void update();
        inline void highlightItem(const QModelIndex &index) {
            ui.fileSystemTree->setCurrentIndex(index);
        }
        inline void expandFolder(const QModelIndex &index) {
            model->fetchMore(index);
        };
        inline void rollUpFolder(const QModelIndex &index) {
            ui.fileSystemTree->collapseAll();
            ui.fileSystemTree->setCurrentIndex(index.parent());
        };
        inline void hideAllItems()
        {
            ui.addressLine->setText("");
            ui.fileSystemTree->setCurrentIndex(QModelIndex());
            ui.fileSystemTree->collapseAll();
        }
        inline void setEditLineText(const QString &text) {
            ui.addressLine->blockSignals(true);
            ui.addressLine->setText(text);
            ui.addressLine->blockSignals(false);
        };
        inline void editItemName(const QModelIndex& index){
            ui.fileSystemTree->edit(index);
        };

    public:
        OfflineFileManagerViewActions actionsSignals;
        OfflineFileManagerViewButtons buttonsSignals;
        OfflineFileManagerViewTree treeSignals;

    private:
        QFileInfoModel *model = nullptr;
        Ui::OfflineFileManagerClass ui;
    };
}
