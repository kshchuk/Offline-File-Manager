#include "propertieslogic.h"

#include <QLocale>
#include <QStandardItem>
#include <QStandardItemModel>

namespace manager
{

    PropertiesLogic::PropertiesLogic(Record* record, QFileInfoModel *model)
    {
        this->record = record;
        this->model = model;

        name = record->getName();
        type = record->getType();
        quint64 l = record->getSizeInBytes().toLongLong();
        QLocale locale;
        size = locale.formattedDataSize(l);
        group = record->getGroup();
        owner = record->getOwner();
        ownerId = record->getOwnerId();
        created = record->getDateCreated();
        last_modified = record->getDateModified();
        customMeta = record->getCustomMetadata();
        fullPath = record->getFullPath();
        hash = record->getHash();
    }

    QString PropertiesLogic::getName() const
    {
        return name;
    }

    QString PropertiesLogic::getType() const
    {
        return type;
    }

    QString PropertiesLogic::getSize() const
    {
        return size;
    }

    QString PropertiesLogic::getGroup() const
    {
        return group;
    }

    QString PropertiesLogic::getOwner() const
    {
        return owner;
    }

    QString PropertiesLogic::getOwnerid() const
    {
        return ownerId;
    }

    QString PropertiesLogic::getCreated() const
    {
        return created;
    }

    QString PropertiesLogic::getLastModified() const
    {
        return last_modified;
    }

    QString PropertiesLogic::getFullPath() const
    {
        return fullPath;
    }

    void PropertiesLogic::setCustomMethadata(QString metadata)
    {
        customMeta = metadata;
    }

    QString PropertiesLogic::getCustomMethadata() const
    {
        return customMeta;
    }

    QString PropertiesLogic::getHash() const
    {
        return hash;
    }

    void PropertiesLogic::saveMeta()
    {
        record->setCustomMetadata(this->customMeta);
    }

    QString PropertiesLogic::getIndeticalCopiesNumber() const
    {
        Searcher searcher(model);
        QVariant *criterias = new QVariant[searcher.criteriasCount];
        criterias[searcher.HASH] = hash;
        connect(&searcher, &Searcher::found, this, &PropertiesLogic::identicalFound);
        searcher.search(criterias, "");
        return QString::number(identicalCopiesNumber);
    }

    void PropertiesLogic::identicalFound()
    {
        identicalCopiesNumber++;
    }

    QList<QStringList> PropertiesLogic::getAll()
    {
        QList<QStringList> all;
        QStringList properties_list = {"Name: ", "Type: ", "Size: ",
                                       "Group: ", "Owner: ", "Owner ID: ", "Created: ", "Last modified: ", "Custom metadata:"};
        QStringList values_list = {getName(), getType(), getSize(), getGroup(),
                                   getOwner(), getOwnerid(), getCreated(), getLastModified(), getCustomMethadata()};
        all.append(properties_list);
        all.append(values_list);
        return all;
    }

}
