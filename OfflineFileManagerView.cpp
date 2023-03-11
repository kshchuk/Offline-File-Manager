#include "OfflineFileManagerView.h"

#include <QScroller>
#include <QScreen>

#include <cassert>

namespace manager
{
    OfflineFileManagerView::OfflineFileManagerView(QWidget *parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        this->setCentralWidget(ui.layoutWidget);

        connect(ui.actionSave, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionSaveToFile);
        connect(ui.actionOpen, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionLoadFromFile);
        connect(ui.actionUpdate, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionUpdate);
        connect(ui.updateButton, &QToolButton::clicked, &this->buttonsSignals, &OfflineFileManagerViewButtons::onUpdateButtonClicked);
        connect(ui.actionReturn_Up_Folder, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionGoUpper);
        connect(ui.upButton, &QToolButton::clicked, &this->buttonsSignals, &OfflineFileManagerViewButtons::onUpButtonClicked);
        connect(ui.actionReturn_Home_Folder, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionGoHome);
        connect(ui.homeButton, &QToolButton::clicked, &this->buttonsSignals, &OfflineFileManagerViewButtons::onHomeButtonClicked);
        connect(ui.actionCreate_virtual_foulder, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionAddFolder);
        connect(ui.addFolderButton, &QToolButton::clicked, &this->buttonsSignals, &OfflineFileManagerViewButtons::onAddFolderButtonClicked);
        connect(ui.actionSearch, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionSearch);
        connect(ui.searchButton, &QToolButton::clicked, &this->buttonsSignals, &OfflineFileManagerViewButtons::onSearchButtonClicked);
        connect(ui.actionrobocopy, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionOpenRobocopy);

        connect(ui.actionExternal_drives, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionExternalDrivesRegime);
        connect(ui.actionAll_drives, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionAllDrivesRegime);
        connect(ui.actionGoogle_Drive, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionGoogleDriveRegime);
        connect(ui.actionGoogle_Drive_2, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionGoogleDriveRegime);

        connect(ui.actionMaximum_depth, &QAction::triggered, &this->actionsSignals, &OfflineFileManagerViewActions::actionSetMaxReadingDepth);

        connect(ui.fileSystemTree, &QTreeView::activated, &this->treeSignals, &OfflineFileManagerViewTree::onTreeWidgetClicked);
        connect(ui.fileSystemTree, &QTreeView::clicked, &this->treeSignals, &OfflineFileManagerViewTree::onTreeWidgetClicked);
        connect(ui.fileSystemTree, &QTreeView::doubleClicked, &this->treeSignals, &OfflineFileManagerViewTree::onTreeWidgetDoubleClicked);

        connect(ui.fileSystemTree, &QTreeView::customContextMenuRequested, &this->treeSignals, &OfflineFileManagerViewTree::onCustomContextMenuRequested);

        connect(ui.addressLine, &QLineEdit::editingFinished, this, &OfflineFileManagerView::onEditLineEditingFinished);
    };

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
}
