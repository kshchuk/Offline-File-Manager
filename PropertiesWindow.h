#pragma once

#include <QTableWidget>
#include <QModelIndex>

#include "PropertiesLogic.h"
#include "QFileInfoModel.h"


class PropertiesWindow : public QTableWidget
{
    Q_OBJECT

    enum PropertiesOrder
    {
        NAME,
        FULL_PATH,
        TYPE,
        SIZE,
        ICON_NAME,
        GROUP,
        OWNER,
        OWNER_ID,
        DATE_CREATED,
        DATE_MODIDFIED,
        MD5,
        CUSTOM_METHADATA,
    };

public:
    explicit PropertiesWindow(QWidget* parent = 0,
        QModelIndex index = QModelIndex(), QFileInfoModel* model = nullptr);

public slots:
    void saveTextSlot();

signals:
    void saveTextSignal(const QString& message, const QModelIndex& index);

private:
    PropertiesLogic* properties;
    QModelIndex index;
};
