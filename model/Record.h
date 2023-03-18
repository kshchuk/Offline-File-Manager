/**
 * @file record.h
 * @brief Contains classes for managing files and directories, and for parsing network responses.
 */

#pragma once

#include <QCryptographicHash>
#include <QFileIconProvider>
#include <QStandardItem>
#include <QMimeDatabase>
#include <QDirIterator>
#include <QLocale>
#include <QString>

namespace manager
{
    QLocale Record::locale = QLocale();
    QMimeDatabase Record::mimeDatabase = QMimeDatabase();
    QFileIconProvider Record::iconProvider = QFileIconProvider();
    QCryptographicHash File::crypto = QCryptographicHash(QCryptographicHash::Md5);

    /**
     * @brief The base class for representing various file records in the file manager application.
     *
     * The class contains common properties such as size, type, date modified, etc., that can be
     * shared between different types of records, such as drives, folders, files, links, virtual folders, etc.
     */
    class Record : public QStandardItem
    {
    public:
        /**
         * @brief Constructs a Record object from an existing QStandardItem object.
         *
         * @param item The QStandardItem object to use.
         */
        Record(const QStandardItem &item);
        virtual ~Record() {}

        /// Getters
        inline QString getName() const { return this->data().toString(); }
        inline QString getSize() const { return size->data().toString(); }
        inline QString getType() const { return type->data().toString(); }
        inline QString getDateModified() const { return dateModified->data().toString(); }
        inline QString getIconName() const { return iconName->data().toString(); }
        inline QString getDateCreated() const { return dateCreated->data().toString(); }
        inline QString getGroup() const { return group->data().toString(); }
        inline QString getOwner() const { return owner->data().toString(); }
        inline QString getOwnerId() const { return ownerId->data().toString(); }
        inline QString getSizeInBytes() const { return sizeInBytes->data().toString(); }
        inline QString getFullPath() const { return fullPath->data().toString(); }
        inline QString getHash() const { return hash->data().toString(); }
        inline QString getCustomMetadata() const { return customMetadata->data().toString(); }

        /// Setters
        inline void setCustomMetadata(const QString &str) { customMetadata->setData(str, Qt::DisplayRole); }
        inline void setName(const QString &name) { this->setText(name); }

    protected:
        /**
         * @brief Constructor for creating a record with a specified icon and name.
         *
         * @param icon The icon to use for the record.
         * @param name The name of the record.
         */
        explicit Record(const QIcon &icon, const QString &name);

        QStandardItem *size, *type, *dateModified, *iconName, *dateCreated,
            *group, *owner, *ownerId, *sizeInBytes, *fullPath, *hash, *customMetadata;

        static QLocale locale;
        static QMimeDatabase mimeDatabase;
        static QFileIconProvider iconProvider;

        /**
         * @brief Computes the size of a file specified by the given QFileInfo object and returns it as a formatted string.
         *
         * @param info The QFileInfo object representing the file.
         * @return The size of the file as a formatted string.
         */
        static inline QString fileSize(const QFileInfo &info);
        /**
         * @brief Formats the specified size in bytes as a human-readable string.
         *
         * @param size The size in bytes.
         * @return The size as a formatted string.
         */
        static inline QString fileSize(qint64 size);
    };

    /**
     * @brief A subclass of Record that represents a drive.
     *
     */
    class Drive : public Record
    {
    public:
        /**
         * @brief Constructs a Drive object for the specified QDirIterator object.
         *
         * @param drive The QDirIterator object representing the drive.
         */
        explicit Drive(const QDirIterator &drive);
        virtual ~Drive() {}
    };

    /**
     * @brief A subclass of Record that represents a folder in the file system.
     *
     */
    class Folder : public Record
    {
    public:
        /**
         * @brief Constructs a Folder object for the specified QFileInfo object representing the folder.
         *
         * @param folder The QFileInfo object representing the folder.
         */
        explicit Folder(const QFileInfo &folder);
        virtual ~Folder() {}
    };

    /**
     * @brief A subclass of Record that represents a file in the file system.
     *
     * This class provides additional functionality for interacting with files, such as computing
     * the file's hash value.
     */
    class File : public Record
    {
    public:
        /**
         * @brief Constructs a File object for the specified QFileInfo object representing the file.
         *
         * @param file The QFileInfo object representing the file.
         */
        explicit File(const QFileInfo &file);
        virtual ~File() {}

    private:
        static QCryptographicHash crypto; /**< The cryptographic hash function used to compute the file's hash value.*/

        /**
         * @brief Computes the hash value for the specified file and returns it as a QByteArray.
         *
         * @param info The QFileInfo object representing the file.
         * @return The hash value as a QByteArray.
         */
        static QByteArray hashFile(const QFileInfo &info);
    };

    /**
     * @brief A class representing a link record.
     * This class extends the Record class and represents a link record. It contains a constructor
     * that takes a QModelIndex as input and sets the necessary fields.
     */
    class Link : public Record
    {
    public:
        /**
         * @brief A class representing a link record.
         * This class extends the Record class and represents a link record. It contains a constructor
         * that takes a QModelIndex as input and sets the necessary fields.
         */
        explicit Link(const QModelIndex &index);
        virtual ~Link() {}
    };

    /**
     * @brief A class representing a virtual folder record.
     * This class extends the Record class and represents a virtual folder record. It contains a constructor
     * that takes a QString representing the folder name as input and sets the necessary fields.
     */
    class VirtualFolder : public Record
    {
    public:
        explicit VirtualFolder(const QString &name);
        virtual ~VirtualFolder() {}
    };

    /**
     * @brief A class representing a network file record.
     * This class extends the Record class and represents a network file record. It contains a constructor
     * that takes a QJsonValue representing the file as input and sets the necessary fields.
     */
    class NetworkFile : public Record
    {
    public:
        explicit NetworkFile(const QJsonValue &file);
        virtual ~NetworkFile() {}
    };

    /**
     * @brief Converts an RFC 3339 date string to a QDateTime object.
     * This function takes a QString representing an RFC 3339 date string as input and returns
     * a QDateTime object representing the same date and time.
     * @param str The RFC 3339 date string to convert
     * @return A QDateTime object representing the input date and time
     */
    QDateTime FromRfc3339(const QString &str);
}
