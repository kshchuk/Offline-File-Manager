#pragma once

#include <QMessageBox>

#include "OfflineFileManagerController.h"


namespace manager {

class SaveMessage
{
public:
    explicit SaveMessage(OfflineFileManagerController* controller) {
            message.setText("Save data");
            message.setInformativeText("You have unsaved changes. Do you want to save it?");
            message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            message.setDefaultButton(QMessageBox::Yes);
            this->controller = controller;
    }

    void show()
    {
            int ret = message.exec();
            switch (ret)
            {
            case QMessageBox::Yes:
                controller->saveModelToFile();
                break;
            case QMessageBox::No:
                break;
            case QMessageBox::Close:
                break;
            default:
                Q_ASSERT(1 == 0);
            }
    };

private:
    OfflineFileManagerController* controller;
    QMessageBox message;
};

}
