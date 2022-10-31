#pragma once

#include <QTableWidget>

class PropertiesWindow : public QTableWidget
{
    Q_OBJECT

public:
    explicit PropertiesWindow(QWidget* parent = 0);

public slots:
    void saveTextSlot();

signals:
    void saveTextSignal(const QString& message);
};
