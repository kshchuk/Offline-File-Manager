/**
 * @file OfflineFileManagerController.h
 * @brief This file contains the declaration of the OfflineFileManagerController class, which is responsible for controlling the offline file manager view and model.
 */

#pragma once

#include "model/QFileInfoModel.h"
#include "model/DrivesModel.h"
#include "OfflineFileManagerView.h"

namespace manager
{
    static constexpr size_t defaultDepth = 5;

    /**
     * @class OfflineFileManagerController
     * @brief The OfflineFileManagerController class is responsible for controlling the offline file manager view and model.
     * This class inherits from QObject and defines several slots and signals that are used to interact with the OfflineFileManagerView and QFileInfoModel objects.
     */
    class OfflineFileManagerController : public QObject
    {
        Q_OBJECT

        /**

        @brief The Mode enum specifies the current mode of the offline file manager.
        */
        enum class Mode
        {
            FILESYSTEM,
            EXTERNAL_DRIVES,
            GOOGLE_DRIVE
        };

    public:
        /**
         * @brief Constructor for the OfflineFileManagerController class.
         *
         * @param model The QFileInfoModel object used to manage the file system.
         * @param view The OfflineFileManagerView object used to display the file system.
         * @param parent The parent QObject for this object.
         */
        explicit OfflineFileManagerController(QFileInfoModel *model, OfflineFileManagerView *view, QObject *parent = nullptr);

    signals:
        /**
         * @brief This signal is emitted when a save operation is requested.
         * @param path The path to which the file system should be saved.
         */
        void save(const QString &);
        /**
         * @brief This signal is emitted when a load operation is requested.
         *
         * @param path The path from which the file system should be loaded.
         * @param maxDepth The maximum depth to which the file system should be loaded.
         */
        void load(const QString &, size_t);

    public slots:
        /**
         * @brief This slot is called when a save operation is requested.
         */
        void saveModelToFile();
        /**
         * @brief This slot is called when a load operation is requested.
         */
        void loadModelFromFile();
        /**
         * @brief This slot is called when the file system should be updated.
         */
        void update();
        /**
         * @brief This slot is called when the parent directory should be displayed.
         */
        void goUpper();
        /**
         * @brief This slot is called when a new virtual folder should be added to the file system tree.
         */
        void addFolder();
        /**
         * @brief This slot is called when the maximum reading depth for the file system should be set.
         */
        void setMaximumReadingDepth();
        /**
         * @brief This slot is called when a search operation is requested.
         */
        void search();
        /**
         * @brief This slot is called when the Robocopy tool should be opened.
         */
        void openRobocopyTool(){
            // TODO
        };
        /**
         * @brief This slot is called when the home directory should be displayed.
         */
        void goHome()
        {
            view->hideAllItems();
        };
        void setExternalDrivesMode()
        {
            mode = Mode::EXTERNAL_DRIVES;
            delete model;
            model = new ExternalDrivesModel();
        };
        void setAllDrivesMode()
        {
            mode = Mode::FILESYSTEM;
            delete model;
            model = new AllDrivesModel();
        };
        void setGoogleDriveMode()
        {
            mode = Mode::GOOGLE_DRIVE;
            delete model;
            model = new GoogleDriveModel();
        };

        void openCustomContextMenu(const QPoint &point);
        void editLineChanged()
        {
            auto path = view->getEditLineText();
            QModelIndex index = model->byPath(path);

            if (index.isValid())
            {
                view->expandFolder(index);
                view->highlightItem(index);
            }
        };

        void alertUser();

    public:
        size_t maxReadingDepth = defaultDepth;

    private:
        QFileInfoModel *model = nullptr;
        OfflineFileManagerView *view = nullptr;

        Mode mode = Mode::EXTERNAL_DRIVES;
    };
}
