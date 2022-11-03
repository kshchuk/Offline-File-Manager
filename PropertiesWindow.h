#pragma once

#include <QTableWidget>
#include <QModelIndex>


class PropertiesWindow : public QTableWidget
{
    Q_OBJECT

public:
    explicit PropertiesWindow(QWidget* parent = 0, QModelIndex index = QModelIndex());

public slots:
    void saveTextSlot();

signals:
    void saveTextSignal(const QString& message, const QModelIndex& index);

private:
    QModelIndex index;
};
