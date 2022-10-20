#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineFileManager.h"

class OfflineFileManager : public QMainWindow
{
    Q_OBJECT

public:
    OfflineFileManager(QWidget *parent = nullptr);
    ~OfflineFileManager();

private:
    Ui::OfflineFileManagerClass ui;
};
