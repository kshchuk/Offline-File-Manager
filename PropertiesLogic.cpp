#include "propertieslogic.h"

#include <QLocale>
#include <QStandardItem>
#include <QStandardItemModel>



PropertiesLogic::PropertiesLogic(const QModelIndex& index, QFileInfoModel* model)
{
    this->index = index;
    this->model = model;

    const int role = 0;

    QModelIndex itemIndex = index.siblingAtColumn(int(ColunmsOrder::NAME));
    QStandardItem* item = model->itemFromIndex(itemIndex);
    name = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::TYPE));
    item = model->itemFromIndex(itemIndex);
    type = item->data(role).toString();
    quint64 l = model->fileSize(index);
    QLocale locale;
    size = locale.formattedDataSize(l);
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::GROUP));
    item = model->itemFromIndex(itemIndex);
    group = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::OWNER));
    item = model->itemFromIndex(itemIndex);
    owner = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::OWNER_ID));
    item = model->itemFromIndex(itemIndex);
    ownerId = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::DATE_CREATED));
    item = model->itemFromIndex(itemIndex);
    created = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::DATE_MODIDFIED));
    item = model->itemFromIndex(itemIndex);
    last_modified = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::CUSTOM_METHADATA));
    item = model->itemFromIndex(itemIndex);
    customMeta = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::FULL_PATH));
    item = model->itemFromIndex(itemIndex);
    fullPath = item->data(role).toString();
    itemIndex = index.siblingAtColumn(int(ColunmsOrder::MD5));
    item = model->itemFromIndex(itemIndex);
    if (item)
    md5Hash = item->data(role).toString();
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

QString PropertiesLogic::getMd5() const
{
    return md5Hash;
}

void PropertiesLogic::saveMeta()
{
    QModelIndex itemIndex = index.siblingAtColumn(int(ColunmsOrder::CUSTOM_METHADATA));
    QStandardItem* item = model->itemFromIndex(itemIndex);
    item->setData(customMeta);
}

QString PropertiesLogic::getIndeticalCopiesNumber() const
{
    Searcher* searcher = new Searcher(model);
    QVariant* criterias = new QVariant[searcher->criteriasCount];
    criterias[searcher->HASH] = md5Hash;
    connect(searcher, &Searcher::found, this, &PropertiesLogic::identicalFound);
    searcher->search(criterias, "");
    return QString::number(identicalCopiesNumber);
}

void PropertiesLogic::identicalFound()
{
    identicalCopiesNumber++;
}


QList<QStringList> PropertiesLogic::getAll()
{
    QList<QStringList> all;
    QStringList properties_list = { "Name: ", "Type: ", "Size: ",
                                   "Group: ", "Owner: ", "Owner ID: ", "Created: ", "Last modified: ", "Custom metadata:"
    };
    QStringList values_list = { getName(), getType(), getSize(), getGroup(),
                                getOwner(), getOwnerid(), getCreated(), getLastModified(), getCustomMethadata()
    };
    all.append(properties_list); all.append(values_list);
    return all;
}