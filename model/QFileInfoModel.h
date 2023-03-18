/**
 * @file qfileinfomodel.h
 * @brief Contains the declaration of classes and functions related to file and folder management.
 */

#pragma once

#include <QStandardItemModel>
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QString>
#include <QList>


namespace manager
{
	/**
	 * @brief An interface for manipulating the files and folders of a model.
	 */
	class FolderFileManipulator
	{
	public:
		/**
		 * @brief Inserts a link to a file into a folder.
		 * @param toInsert The index of the link to insert.
		 * @param destination The index of the folder to insert into.
		 */
		virtual void insertFileLinkToTheFolder(QModelIndex toInsert, QModelIndex destination) = 0;

		/**
		 * @brief Inserts a file into a folder.
		 * @param path The path of the file to insert.
		 * @param destination The index of the folder to insert into.
		 */
		virtual void insertFileToTheFolder(const QString &path, QModelIndex destination) = 0;

		/**
		 * @brief Appends a virtual folder to a parent folder.
		 * @param parent The index of the parent folder to append to.
		 * @return The index of the created folder.
		 */
		virtual QModelIndex appendVirtualFolder(QModelIndex parent) = 0;

		/**
		 * @brief Deletes a file or folder.
		 * @param index The index of the file or folder to delete.
		 */
		virtual void deleteFile(const QModelIndex &index) = 0;
	};

	/**
	 * @brief An interface for accessing information about files and folders in a model.
	 */
	class FileInfoModelHelper
	{
	public:
		/**
		 * @brief Gets the path to a file or folder (generates path based on file location in a tree).
		 * @param index The index of the file or folder.
		 * @return The path to the file or folder.
		 */
		virtual QList<QString> getPath(QModelIndex index) const noexcept = 0;

		/**
		 * @brief Gets the path to a file or folder from its information.
		 * @param index The index of the file or folder.
		 * @return The path to the file or folder.
		 */
		virtual QString getPathFromInfo(const QModelIndex &index) const noexcept = 0;

		/**
		 * @brief Gets the index of a file or folder given its path.
		 * @param path The path of the file or folder.
		 * @return The index of the file or folder.
		 */
		virtual QModelIndex byPath(QString path) const noexcept = 0;

		/**
		 * @brief Converts a string list to a path string.
		 * @param list The list of path segments.
		 * @return The path string.
		 */
		static QString pathFromStringList(const QStringList &list);

		/**
		 * @brief Checks if an index is a link to a file.
		 * @param index The index to check.
		 * @return Whether the index is a link to a file.
		 */
		virtual bool isLink(const QModelIndex &index) const = 0;

		/**
		 * @brief Checks if an index is a folder.
		 * @param index The index to check.
		 * @return Whether the index is a Folder
		 */
		virtual bool isFolder(const QModelIndex &index) const = 0;
	};

	/**
	 * @brief The ModelIO class is an abstract base class that defines an interface for reading and writing QAbstractItemModel objects to and from files.
	 */
	class ModelIO
	{
	public:
		/**
		 * @brief readFile is a pure virtual function that must be implemented by derived classes. It reads a QAbstractItemModel object from a file specified by fileName and returns a pointer to it.
		 * @param fileName The name of the file to read the model from.
		 * @return A pointer to the QAbstractItemModel object read from the file.
		 * @throw std::runtime_error if the file cannot be opened or if there is a reading error.
		 */
		virtual QAbstractItemModel *readFile(QString fileName) = 0;

		/**
		 * @brief writeFile is a pure virtual function that must be implemented by derived classes. It writes the contents of the model to a file specified by fileName, down to a maximum depth specified by maxDepth.
		 * @param fileName The name of the file to write the model to.
		 * @param maxDepth The maximum depth of the model to write to the file.
		 * @throw std::runtime_error if the file cannot be opened or if there is a writing error.
		 */
		virtual void writeFile(QString fileName, size_t maxDepth) const = 0;
	};

	/**
	 * @brief The QFileInfoModel class represents a model for displaying file and folder information
	 * with additional functionalities such as file manipulation, file system access and file info queries.
	 * @details The QFileInfoModel class inherits from QStandardItemModel and implements the FolderFileManipulator,
	 * ModelIO, and FileInfoModelHelper interfaces.
	 * @note This class is not thread-safe.
	 * @see QStandardItemModel, FolderFileManipulator, ModelIO, FileInfoModelHelper
	 */
	class QFileInfoModel : public QStandardItemModel, public FolderFileManipulator, public ModelIO, public FileInfoModelHelper
	{

		Q_OBJECT

	public:
		/**
		 * @brief Constructs a new QFileInfoModel object with a parent object.
		 * @param parent the parent QObject
		 */
		explicit QFileInfoModel(QObject *parent = nullptr);
		/**
		 * @brief Destructs the QFileInfoModel object.
		 */
		virtual ~QFileInfoModel() {}

		/**
		 * @brief Returns the path of the specified index.
		 *
		 * @param index the QModelIndex of the item
		 * @return QList<QString> the list of strings representing the path of the item
		 */
		QList<QString> getPath(QModelIndex index) const noexcept override;
		/**
		 * @brief Returns the path of the item represented by the given QModelIndex.
		 *
		 * @param index the QModelIndex of the item
		 * @return QString the path of the item
		 */
		QString getPathFromInfo(const QModelIndex &index) const noexcept override;
		/**
		 * @brief Returns the QModelIndex corresponding to the item with the given path.
		 *
		 * @param path the path of the item
		 * @return QModelIndex the QModelIndex corresponding to the item
		 */
		QModelIndex byPath(QString path) const noexcept override;

		/**
		 * @brief Converts a QStringList representing a path to a single QString.
		 *
		 * @param list the list of strings representing the path
		 * @return QString the path as a single string
		 */
		static QString pathFromStringList(const QStringList &list);

		/**
		 * @brief Returns true if the item represented by the given QModelIndex is a symbolic link.
		 *
		 * @param index the QModelIndex of the item
		 * @return bool true if the item is a symbolic link
		 */
		bool isLink(const QModelIndex &index) const override;
		/**
		 * @brief Returns true if the item represented by the given QModelIndex is a folder.
		 *
		 * @param index the QModelIndex of the item
		 * @return bool true if the item is a folder
		 */
		bool isFolder(const QModelIndex &index) const override;

		/**
		 * @brief Inserts a file link into the specified folder.
		 *
		 * @param toInsert the QModelIndex of the item to insert
		 * @param destination the QModelIndex of the destination folder
		 */
		void insertFileLinkToTheFolder(QModelIndex toInsert, QModelIndex destination) override;
		/**
		 * @brief Inserts a file into the specified folder.
		 *
		 * @param path the path of the file to insert
		 * @param destination the QModelIndex of the destination folder
		 */
		void insertFileToTheFolder(const QString &path, QModelIndex destination) override;
		/**
		 * @brief Appends a virtual folder to the specified parent item.
		 *
		 * @param parent the QModelIndex of the parent item
		 * @return QModelIndex the QModelIndex of the new virtual folder
		 */
		QModelIndex appendVirtualFolder(QModelIndex parent) override;
		/**
		 * @brief Deletes the item represented by the given QModelIndex.
		 *
		 * @param index the QModelIndex of the item to delete
		 */
		void deleteFile(const QModelIndex &index) override;

		/**
		 * @brief Sets the name of the item at the given index.
		 *
		 * @param newName The new name.
		 * @param index The index of the item.
		 */
		inline void setName(QString newName, QModelIndex index);

		/**
		 * @brief Returns the size of the file or the folder represented by the given index.
		 *
		 * @param index The index of the file.
		 * @return The size of the file.
		 */
		quint64 fileSize(const QModelIndex &index) const;

	signals:
		/**
		 * @brief This signal is emitted when the model has finished loading its data.
		 */
		void loaded();

	public slots:

		/**
		 * @brief A virtual pure method that generates a QFileInfoModel object with files and folders up to the given maximum depth.
		 *
		 * @param maxDepth The maximum depth of the files and folders.
		 * @return A new QFileInfoModel object.
		 */
		virtual manager::QFileInfoModel *generate(size_t maxDepth) = 0;

		/**
		 * @brief Reads the specified file into a QAbstractItemModel object.
		 *
		 * @param fileName The name of the file to read.
		 * @return A new QAbstractItemModel object.
		 */
		QAbstractItemModel *readFile(QString fileName) override;

		/**
		 * @brief Writes the QFileInfoModel object to the specified file with files and folders up to the given maximum depth.
		 *
		 * @param fileName The name of the file to write to.
		 * @param maxDepth The maximum depth of the files and folders.
		 */
		void writeFile(QString fileName, size_t maxDepth) const override;

	protected:
		/**
		 * @brief Sets the icons for the all items in the model.
		 *
		 * @param index The QModelIndex of the item.
		 * @param depth The depth of tree.
		 */
		void setIcons(const QModelIndex &index = QModelIndex(), int depth = 0);
		/**
		 * @brief Initializes the headers of the model.
		 */
		void initHeaders();

		static QFileSystemModel system_model;

	private:
		/**
		 * @brief Reads files and folders in the system recursive.
		 *
		 * @param piece An iterator to the beginning of the path list.
		 * @param end An iterator to the end of the path list.
		 * @param parent The parent QModelIndex.
		 * @return The QModelIndex corresponding to the path.
		 */
		QModelIndex byPathRecursive(QStringList::const_iterator piece,
									QStringList::const_iterator end, const QModelIndex &parent) const;

		static QFileIconProvider iconProvider;
	};
}
