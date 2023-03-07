#pragma once

#include <QCryptographicHash>
#include <QStandardItemModel>
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QStorageInfo>
#include <QMimeDatabase>
#include <QStandardItem>
#include <QDirIterator>
#include <QFileInfo>
#include <QLocale>
#include <QString>
#include <QList>
#include <QIcon>


namespace manager
{
	 
	//const size_t columnsNumber = 12;

	static const QString virtualFolderType = "Virtual folder";
	static const QString linkToFileType = "Link";

	class FolderFileManipulator
	{
	public:
		virtual void insertFileLinkToTheFolder(QModelIndex toInsert, QModelIndex destination) = 0;
		virtual void insertFileToTheFolder(const QString& path, QModelIndex destination) = 0;
		virtual QModelIndex appendVirtualFolder(const QFileInfo& info, QModelIndex parent) = 0;
		virtual void deleteFile(const QModelIndex& index) = 0;
	};


	class ModelIO 
	{
	public:
		virtual QAbstractItemModel* readFile(QString fileName) = 0;
		virtual void writeFile(QString fileName, size_t maxDepth) const = 0;
	};



	class QFileInfoModel : public QStandardItemModel, public FolderFileManipulator, public ModelIO
	{

		Q_OBJEC

	public:
		explicit QFileInfoModel(QObject* parent = nullptr);
		virtual ~QFileInfoModel() {}

		QList<QString> getPath(QModelIndex index) const noexcept;
		inline QString getPathfFromInfo(const QModelIndex& index) const noexcept;
		QModelIndex byPath(QString path) const noexcept;
		static QString pathFromStringList(const QStringList& list);

		inline void insertFileLinkToTheFolder(QModelIndex toInsert, QModelIndex destination) override;
		inline void insertFileToTheFolder(const QString& path, QModelIndex destination) override;
		QModelIndex appendVirtualFolder(const QFileInfo& info, QModelIndex parent) override;
		void deleteFile(const QModelIndex& index) override;

		inline void setName(QString newName, QModelIndex index);

		inline bool isLink(const QModelIndex& index) const;
		inline bool isFolder(const QModelIndex& index) const;

		quint64 fileSize(const QModelIndex& index) const;

	signals:
		void loaded();

	protected:
		void setIcons(const QModelIndex& index = QModelIndex(), int depth = 0);
		void initHeaders();

		static QFileSystemModel system_model;

	private slots:
		virtual QFileInfoModel* generate(size_t maxDepth) = 0;

		QAbstractItemModel* readFile(QString fileName) override;
		void writeFile(QString fileName, size_t maxDepth) const override;


	private:
		QModelIndex byPathRecursive(QStringList::const_iterator piece,
		QStringList::const_iterator end, const QModelIndex& parent) const;

		static QFileIconProvider iconProvider;
	};


	class DrivesModel : public QFileInfoModel
	{
		Q_OBJECT

	public:
		explicit DrivesModel(QObject* parent = nullptr)
			: QFileInfoModel(parent) {}
		virtual ~DrivesModel() {}


	signals:
		void currentReadingFile(const QString& path);
		void fileRead(int currentPercentage);

	private slots:
		DrivesModel* generate(size_t maxDepth) override = 0;

	protected:
		void readFilesRecursive(QModelIndex parent, const QString& path,
				size_t maxDepth, size_t curDepth = 1);
		quint64 allSize = 0;
		quint64 readSize = 0;
		bool isRunning = true;

		static QStorageInfo storageInfo;
	};


	class ExternalDrivesModel : public DrivesModel
	{
		Q_OBJECT

	public:
		explicit ExternalDrivesModel(QObject* parent = nullptr)
			: DrivesModel(parent) {}

		virtual ~ExternalDrivesModel() {}

	private slots:
		ExternalDrivesModel* generate(size_t maxDepth) override;
	};


	class AllDrivesModel : public DrivesModel
	{
		Q_OBJECT

	public:
		explicit AllDrivesModel(QObject* parent = nullptr)
			: DrivesModel(parent) {}

		virtual	~AllDrivesModel() {}

	private slots:
		AllDrivesModel* generate(size_t maxDepth) override;
	};



	class GoogleDriveModel : public QFileInfoModel
	{
		Q_OBJECT

	public:
		explicit GoogleDriveModel(QObject* parent = nullptr)
			: QFileInfoModel(parent) {}

		virtual ~GoogleDriveModel() {}

	private slots:
		GoogleDriveModel* generate(size_t maxDepth) override;

	private:
		void generateFromResponse(const QJsonDocument& response);
	};
	

	class Record : public QStandardItem
	{
	public:
		Record(const QStandardItem& item);
		virtual ~Record() {}

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

		inline void setCustomMetadata(const QString& str) { customMetadata->setData(str, Qt::DisplayRole); }
		inline void setName(const QString& name) { this->setText(name); }

	protected:
		explicit Record(const QIcon& icon, const QString& name);

		QStandardItem *size, *type, *dateModified, *iconName, *dateCreated,
			*group, *owner, *ownerId, *sizeInBytes, *fullPath, *hash, *customMetadata;

		static QLocale locale;
		static QMimeDatabase mimeDatabase;
		static QFileIconProvider iconProvider;

		static inline QString fileSize(const QFileInfo& info);
		static inline QString fileSize(qint64 size);
	};


	class Drive : public Record
	{
	public:
		explicit Drive(const QDirIterator& drive);
		virtual ~Drive() { }
	};


	class Folder : public Record
	{
	public:
		explicit Folder(const QFileInfo& folder);
		virtual ~Folder() { }
	};


	class File : public Record
	{
	public:
		explicit File(const QFileInfo& file);
		virtual ~File() {}
	
	private:
		static 	QCryptographicHash crypto;

		static QByteArray hashFile(const QFileInfo& info);
	};

	QCryptographicHash File::crypto(QCryptographicHash::Md5);


	class Link : public Record
	{
	public:
		explicit Link(const QModelIndex& index);
		virtual ~Link() {}
	};


	class VirtualFolder : public Record
	{
	public:
		explicit VirtualFolder(const QString& name);
		virtual ~VirtualFolder() {}
	};


	class NetworkFile : public Record
	{
	public:
		explicit NetworkFile(const QJsonValue& file);
		virtual ~NetworkFile() {}
	};


	QDateTime FromRfc3339(const QString& str);
}