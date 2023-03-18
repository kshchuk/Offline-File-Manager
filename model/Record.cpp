#include "Record.h"

#include <QJsonValue>

namespace manager
{
    static const QString virtualFolderType = "Virtual folder";
    static const QString linkToFileType = "Link";

    QDateTime FromRfc3339(const QString &s)
    {
        auto datetime = s.split("T");
        datetime.append(datetime[1].split("."));
        datetime.removeAt(1);
        datetime.removeLast();
        QString str;
        foreach (const auto &t, datetime)
            str += t + " ";
        auto date = datetime[0].split("-");
        auto time = datetime[1].split(":");
        QDateTime dt(QDate(date[0].toInt(), date[1].toInt(), date[2].toInt()), QTime(time[0].toInt(), time[1].toInt(), time[2].toInt()));
        return dt;
    }

    Record::Record(const QStandardItem &item)
        : QStandardItem(item)
    {
        assert(item.columnCount() == 13);

        this->setText(item.data(Qt::DisplayRole).toString());
        this->size = item.model()->itemFromIndex(item.index().siblingAtColumn(1));
        this->type = item.model()->itemFromIndex(item.index().siblingAtColumn(2));
        this->dateModified = item.model()->itemFromIndex(item.index().siblingAtColumn(3));
        this->iconName = item.model()->itemFromIndex(item.index().siblingAtColumn(4));
        this->dateCreated = item.model()->itemFromIndex(item.index().siblingAtColumn(5));
        this->group = item.model()->itemFromIndex(item.index().siblingAtColumn(6));
        this->owner = item.model()->itemFromIndex(item.index().siblingAtColumn(7));
        this->ownerId = item.model()->itemFromIndex(item.index().siblingAtColumn(8));
        this->sizeInBytes = item.model()->itemFromIndex(item.index().siblingAtColumn(9));
        this->fullPath = item.model()->itemFromIndex(item.index().siblingAtColumn(10));
        this->hash = item.model()->itemFromIndex(item.index().siblingAtColumn(11));
        this->customMetadata = item.model()->itemFromIndex(item.index().siblingAtColumn(12));
    }

    Record::Record(const QIcon &icon, const QString &name)
        : QStandardItem(icon, name)
    {
        this->appendRow({size = new QStandardItem(),
                                    type = new QStandardItem(),
                                    dateModified = new QStandardItem(),
                                    iconName = new QStandardItem(),
                                    dateCreated = new QStandardItem(),
                                    group = new QStandardItem(),
                                    owner = new QStandardItem(),
                                    ownerId = new QStandardItem(),
                                    sizeInBytes = new QStandardItem(),
                                    fullPath = new QStandardItem(),
                                    hash = new QStandardItem(),
                                    customMetadata = new QStandardItem()});
    }

    QString Record::fileSize(const QFileInfo &info)
    {
        if (info.isFile())
        {
            quint64 size = info.size();
            return locale.formattedDataSize(size);
        }
        return QString();
    }

    inline QString Record::fileSize(qint64 size)
    {
        return locale.formattedDataSize(size);
    }

    QByteArray File::hashFile(const QFileInfo &info)
    {
        QByteArray qb;
        qlonglong size = info.size();
        qb.setNum(size);
        crypto.addData(qb);

        QString path = info.filePath();
        QFile file(path);

        const int steps = 10;
        const int chunkSize = 10000;
        const qlonglong chunkStep = size / steps;

        crypto.addData(file.read(chunkSize));
        file.seek(size - chunkSize);
        crypto.addData(file.read(chunkSize));

        if (chunkSize <= size)
        {
            qlonglong cur = chunkSize;
            file.seek(cur);
            for (size_t i = 0; i < steps - 2; ++i)
            {
                crypto.addData(file.read(chunkSize));
                if (chunkSize * i < size)
                    file.seek(chunkSize * i);
                else
                    break;
            }
        }

        QByteArray hash = crypto.result();
        file.close();
        return hash;
    }

    Drive::Drive(const QDirIterator &drive)
        : Record(iconProvider.icon(QFileIconProvider::Drive), drive.fileName())
    {
        size->setText(fileSize(drive.fileInfo()));
        type->setText(mimeDatabase.mimeTypeForFile(drive.path()).comment());
        dateModified->setText(drive.fileInfo().lastModified().toString());
        iconName->setText(mimeDatabase.mimeTypeForFile(drive.path()).iconName());
        dateCreated->setText(drive.fileInfo().birthTime().toString());
        group->setText(drive.fileInfo().group());
        owner->setText(drive.fileInfo().owner());
        ownerId->setText(QString::number(drive.fileInfo().ownerId()));
        sizeInBytes->setText(QString::number(drive.fileInfo().size()));
        fullPath->setText(drive.fileInfo().absoluteFilePath());
    }

    Folder::Folder(const QFileInfo &folder)
        : Record(iconProvider.icon(QFileIconProvider::Folder), folder.fileName())
    {
        size->setText(fileSize(folder));
        type->setText(mimeDatabase.mimeTypeForFile(folder).comment());
        dateModified->setText(folder.lastModified().toString());
        iconName->setText(iconProvider.icon(QFileIconProvider::Folder).name());
        dateCreated->setText(folder.birthTime().toString());
        group->setText(folder.group());
        owner->setText(folder.owner());
        ownerId->setText(QString::number(folder.ownerId()));
        sizeInBytes->setText(QString::number(folder.size()));
        fullPath->setText(folder.absoluteFilePath());
    }

    File::File(const QFileInfo &file)
        : Record(iconProvider.icon(file), file.fileName())
    {
        size->setText(fileSize(file));
        type->setText(mimeDatabase.mimeTypeForFile(file).comment());
        dateModified->setText(file.lastModified().toString());
        iconName->setText(iconProvider.icon(file).name());
        dateCreated->setText(file.birthTime().toString());
        group->setText(file.group());
        owner->setText(file.owner());
        ownerId->setText(QString::number(file.ownerId()));
        sizeInBytes->setText(QString::number(file.size()));
        fullPath->setText(file.absoluteFilePath());
        hash->setText(hashFile(file));
    }

    Link::Link(const QModelIndex &index)
        : Record(*this->model()->itemFromIndex(index))

    {
        this->setIcon(iconProvider.icon(QFileInfo("example.lnk")));
        this->type->setText(linkToFileType);
    }

    VirtualFolder::VirtualFolder(const QString &name)
        : Record(iconProvider.icon(QFileIconProvider::Folder), name)
    {
        type->setText(virtualFolderType);
        dateCreated->setText(QDateTime::currentDateTime().toString());
    }

    NetworkFile::NetworkFile(const QJsonValue &file)
        : Record(iconProvider.icon(QFileInfo(file["name"].toString())), file["name"].toString())
    {
        size->setText(fileSize(file["size"].toString().toLongLong()));
        type->setText(mimeDatabase.mimeTypeForFile(QFileInfo(file["name"].toString())).comment());
        dateModified->setText(FromRfc3339(file["modifiedTime"].toString()).toString());
        iconName->setText(file["mimeType"].toString());
        dateCreated->setText(FromRfc3339(file["createdTime"].toString()).toString());
        owner->setText(file["owners"][0]["emailAddress"].toString());
        ownerId->setText(file["permissionId"].toString());
        sizeInBytes->setText(file["size"].toString());
        fullPath->setText(file["webViewLink"].toString());
        hash->setText(file["md5Checksum"].toString());
    };
}