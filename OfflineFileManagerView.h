#pragma once

#include <QtWidgets/QMainWindow>

#include <QMessageBox>
#include <QMenu>
#include <QList>

#include "ui_OfflineFileManager.h"
#include "QFileInfoModel.h"


namespace manager
{
    class OfflineFileManagerView : public QMainWindow
    {
       Q_OBJECT

    public:
        explicit OfflineFileManagerView(QWidget* parent = nullptr);

        void setModel(QFileInfoModel* model);

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

        void onTreeWidgetClicked(const QModelIndex&);
        void onTreeWidgetDoubleClicked(const QModelIndex&);
        void onCustomContextMenuRequested(const QPoint& point);
        void onUpdateButtonClicked();
        void onHomeButtonClicked();
        void onUpButtonClicked();
        void onAddFolderButtonClicked();
        void onSearchButtonClicked();

        void onEditLineEditingFinished();
                
    private slots:
        void update();
        void expandFolder(const QModelIndex&);
        void rollUpFolder(const QModelIndex&);
        void setEditLineText(const QString& text);

    private:
        QFileInfoModel* model = nullptr;
        Ui::OfflineFileManagerClass ui;
    };
}
