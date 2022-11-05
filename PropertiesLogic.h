#pragma once

#include <QString>
#include <QList>
#include <QStringList>
#include <QModelIndex>

#include "QFileInfoModel.h"
#include "Searcher.h"


class PropertiesLogic : public QObject
{
    Q_OBJECT

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
    long identicalCopiesNumber = -1;
    QString fullPath;
    QString md5Hash;

    QModelIndex index;
    QFileInfoModel* model;

private slots:
    void identicalFound();

public:
    PropertiesLogic(const QModelIndex& index, QFileInfoModel* model);

    QString getName() const;
    QString getType() const;
    QString getSize() const;
    QString getGroup() const;
    QString getOwner() const;
    QString getOwnerid() const;
    QString getCreated() const;
    QString getLastModified() const;
    QString getFullPath() const;
    void setCustomMethadata(QString methadata);
    QString getCustomMethadata() const;
    QString getMd5() const;
    QString getIndeticalCopiesNumber() const;

    QList<QStringList> getAll(); // atributes and value

    void saveMeta();
};

