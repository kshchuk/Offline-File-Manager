#pragma once

#include <QString>
#include <QList>
#include <QStringList>
#include <QModelIndex>

#include "QFileInfoModel.h"


class PropertiesLogic
{
private:
    QString name;
    QString type;
    QString size;
    QString group;
    QString owner;
    QString ownerId;
    QString created;
    QString last_modified;
    QString customMeta;
    QString iconName;

    QModelIndex index;
    QFileInfoModel* model;

public:
    PropertiesLogic(const QModelIndex& index, QFileInfoModel* model);

    QString getName();
    QString getType();
    QString getSize();
    QString getGroup();
    QString getOwner();
    QString getOwnerid();
    QString getCreated();
    QString getLastModified();
    QString getIconName();
    void setCustomMethadata(QString methadata);
    QString getCustomMethadata();

    QList<QStringList> getAll(); // atributes and value

    void saveMeta();
};

