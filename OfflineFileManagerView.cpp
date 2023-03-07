#include "OfflineFileManagerView.h"

#include <QScroller>
#include<QScreen>

#include <cassert>


namespace manager
{
    OfflineFileManagerView::OfflineFileManagerView(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        connect(ui.actionSave, &QAction::triggered, this, &OfflineFileManagerView::actionSaveToFile);
        connect(ui.actionOpen, &QAction::triggered, this, &OfflineFileManagerView::actionLoadFromFile);
        connect(ui.actionUpdate, &QAction::triggered, this, &OfflineFileManagerView::actionUpdate);
        connect(ui.updateButton, &QToolButton::clicked, this, &OfflineFileManagerView::onUpdateButtonClicked);
        connect(ui.actionReturn_Up_Folder, &QAction::triggered, this, &OfflineFileManagerView::actionGoUpper);
        connect(ui.upButton, &QToolButton::clicked, this, &OfflineFileManagerView::onUpButtonClicked);
        connect(ui.actionReturn_Home_Folder, &QAction::triggered, this, &OfflineFileManagerView::actionGoHome);
        connect(ui.homeButton, &QToolButton::clicked, this, &OfflineFileManagerView::onHomeButtonClicked);
        connect(ui.actionCreate_virtual_foulder, &QAction::triggered, this, &OfflineFileManagerView::actionAddFolder);
        connect(ui.addFolderButton, &QToolButton::clicked, this, &OfflineFileManagerView::onAddFolderButtonClicked);
        connect(ui.actionSearch, &QAction::triggered, this, &OfflineFileManagerView::actionSearch);
        connect(ui.searchButton, &QToolButton::clicked, this, &OfflineFileManagerView::onSearchButtonClicked);
        connect(ui.actionrobocopy, &QAction::triggered, this, &OfflineFileManagerView::actionOpenRobocopy);

        connect(ui.actionExternal_drives, &QAction::triggered, this, &OfflineFileManagerView::actionExternalDrivesRegime);
        connect(ui.actionAll_drives, &QAction::triggered, this, &OfflineFileManagerView::actionAllDrivesRegime);
        connect(ui.actionGoogle_Drive, &QAction::triggered, this, &OfflineFileManagerView::actionGoogleDriveRegime);
        connect(ui.actionGoogle_Drive_2, &QAction::triggered, this, &OfflineFileManagerView::actionGoogleDriveRegime);

        connect(ui.actionMaximum_depth, &QAction::triggered, this, &OfflineFileManagerView::actionSetMaxReadingDepth);

        connect(ui.fileSystemTree, &QTreeView::activated, this, &OfflineFileManagerView::onTreeWidgetClicked);
        connect(ui.fileSystemTree, &QTreeView::clicked, this, &OfflineFileManagerView::onTreeWidgetClicked);
        connect(ui.fileSystemTree, &QTreeView::doubleClicked, this, &OfflineFileManagerView::onTreeWidgetDoubleClicked);

        connect(ui.fileSystemTree, &QTreeView::customContextMenuRequested, this, &OfflineFileManagerView::onCustomContextMenuRequested);
        connect(ui.fileSystemTree, &QTreeView::customContextMenuRequested, this, &OfflineFileManagerView::onCustomContextMenuRequested);

        connect(ui.addressLine, &QLineEdit::editingFinished, this, &OfflineFileManagerView::onEditLineEditingFinished);
    };

    void OfflineFileManagerView::setModel(QFileInfoModel *model)
    {
        this->model = model;
        ui.fileSystemTree->setModel(model);
        this->update();
    }

    void OfflineFileManagerView::update()
    {
        assert(model != nullptr);

        ui.fileSystemTree->setModel(model);

        for (size_t i = 4; i < ui.fileSystemTree->model()->columnCount(); i++)
            ui.fileSystemTree->hideColumn(i); // only 4 columns need to be displayed

        ui.fileSystemTree->setAnimated(false);
        ui.fileSystemTree->setIndentation(20);
        ui.fileSystemTree->setSortingEnabled(true);
        const QSize availableSize = ui.fileSystemTree->screen()->availableGeometry().size();
        ui.fileSystemTree->setColumnWidth(0, ui.fileSystemTree->width() / 2);
        QScroller::grabGesture(ui.fileSystemTree, QScroller::TouchGesture);
        ui.fileSystemTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui.fileSystemTree->setContextMenuPolicy(Qt::CustomContextMenu);
    }

    void OfflineFileManagerView::expandFolder(const QModelIndex &index)
    {
        model->fetchMore(index);
    }

    void OfflineFileManagerView::rollUpFolder(const QModelIndex &)
    {
        ui.addressLine->setText("");
        ui.fileSystemTree->setCurrentIndex(QModelIndex());
        ui.fileSystemTree->collapseAll();
    }

    void OfflineFileManagerView::setEditLineText(const QString &text)
    {
        ui.addressLine->setText(text);
    }
}