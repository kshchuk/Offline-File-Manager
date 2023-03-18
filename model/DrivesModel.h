/**
 * @file DrivesModel.h
 * @brief Header file for the DrivesModel class and its subclasses.
 */

#pragma once

#include "QFileInfoModel.h"

#include <QStorageInfo>
#include <QCryptographicHash>
#include <QMimeDatabase>

namespace manager
{

    /**
     * @brief The DrivesModel class represents a base class for models that generate a list of files and directories
     * from storage devices.
     * The DrivesModel class inherits from the QFileInfoModel class, and it provides a common interface for generating
     * a file list from various storage devices, such as external drives or all drives. It also defines signals for
     * reporting progress during file reading.
     * @note This class is an abstract class, and it cannot be instantiated directly.
     * @see QFileInfoModel
     */
    class DrivesModel : public QFileInfoModel
    {
        Q_OBJECT

    public:
        /**
         * @brief Constructs a DrivesModel object with a given parent.
         * @param parent The QObject parent of the model.
         */
        explicit DrivesModel(QObject *parent = nullptr)
            : QFileInfoModel(parent) {}
        virtual ~DrivesModel() {}

    signals:
        /**
         * @brief Signal emitted when the model is reading a file.
         * @param path The path of the file being read.
         */
        void currentReadingFile(const QString &path);

        /**
         * @brief Signal emitted when a file has been read.
         * @param currentPercentage The current percentage of the model that has been read.
         */
        void fileRead(int currentPercentage);

    public slots:
        /**
         * @brief Generates a model of file information for drives.
         * @param maxDepth The maximum depth to traverse the directory tree.
         * @return A pointer to a QFileInfoModel object representing the drive.
         */
        manager::QFileInfoModel *generate(size_t maxDepth) override = 0;

    protected:
        /**
         * @brief Recursively reads files in a directory.
         * @param parent The parent QModelIndex.
         * @param path The path to the directory.
         * @param maxDepth The maximum depth to traverse the directory tree.
         * @param curDepth The current depth of the traversal.
         */
        void readFilesRecursive(QModelIndex parent, const QString &path,
                                size_t maxDepth, size_t curDepth = 1);

        quint64 allSize = 0;   /**< The total size of all files in the model.*/
        quint64 readSize = 0;  /**< The current size of all files read by the model.*/
        bool isRunning = true; /**< A boolean value indicating if the model is currently reading.*/

        static QStorageInfo storageInfo; /**< A static QStorageInfo object used to query storage information.*/
    };

    /**
     * @brief The ExternalDrivesModel class is a subclass of DrivesModel that generates a model of file information
     * for external drives.
     * @see DrivesModel
     */
    class ExternalDrivesModel : public DrivesModel
    {
        Q_OBJECT

    public:
        /**
         * @brief Constructs an ExternalDrivesModel object with a given parent.
         * @param parent The QObject parent of the model.
         */
        explicit ExternalDrivesModel(QObject *parent = nullptr)
            : DrivesModel(parent) {}
        virtual ~ExternalDrivesModel() {}

    public slots:
        /**
         * @brief Generates a model of file information for external drives.
         * @param maxDepth The maximum depth to traverse the directory tree.
         * @return A pointer to a QFileInfoModel object representing the external drive.
         */
        manager::QFileInfoModel *generate(size_t maxDepth) override;
    };

    /**
     * @brief The AllDrivesModel class is a subclass of DrivesModel that generates a model of file information for
     * all drives.
     * @see DrivesModel
     */
    class AllDrivesModel : public DrivesModel
    {
        Q_OBJECT

    public:
        /**
         * @brief Constructs an AllDrivesModel object with the given parent.
         * @param parent The QObject parent of the AllDrivesModel object.
         */
        explicit AllDrivesModel(QObject *parent = nullptr)
            : DrivesModel(parent) {}

        virtual ~AllDrivesModel() {}

    public slots:
        /**
         * @brief Generates the QFileInfoModel for all drives.
         * @param maxDepth The maximum depth to scan for files and directories.
         * @return A manager::QFileInfoModel representing all drives.
         *
         * This method scans all drives (local and external) for files and directories up to the given maximum depth.
         * It returns a QFileInfoModel representing all the scanned drives.
         */
        manager::QFileInfoModel *generate(size_t maxDepth) override;
    };

    /**
     * @brief A QFileInfoModel for Google Drive.
     *
     * This class extends QFileInfoModel to provide a QFileInfoModel for Google Drive.
     * It overrides the generate() method to retrieve file information from Google Drive API.
     *
     * @see QFileInfoModel
     */
    class GoogleDriveModel : public QFileInfoModel
    {
        Q_OBJECT

    public:
        /**
         * @brief Constructs a GoogleDriveModel object with the given parent.
         * @param parent The QObject parent of the GoogleDriveModel object.
         */
        explicit GoogleDriveModel(QObject *parent = nullptr)
            : QFileInfoModel(parent) {}

        virtual ~GoogleDriveModel() {}

    public slots:
        /**
         * @brief Generates the QFileInfoModel for Google Drive.
         * @param maxDepth The maximum depth to retrieve file information.
         * @return A manager::QFileInfoModel representing Google Drive.
         *
         * This method retrieves file information from Google Drive API up to the given maximum depth.
         * It returns a QFileInfoModel representing Google Drive.
         */
        manager::QFileInfoModel *generate(size_t maxDepth) override;

    private:
        /**
         * @brief Generates QFileInfoModel from the Google Drive API response.
         * @param response The JSON response from the Google Drive API.
         *
         * This method generates a QFileInfoModel from the JSON response received from the Google Drive API.
         */
        void generateFromResponse(const QJsonDocument &response);
    };
}