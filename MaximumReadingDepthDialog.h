#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

#include "OfflineFileManagerController.h"

namespace manager
{


class MaximumReadingDepthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaximumReadingDepthDialog(OfflineFileManagerController *controller) : controller(controller)
    {
        layout = new QVBoxLayout(this);
        label = new QLabel(this);
        label->setText("Maximum depth: ");
        box = new QSpinBox(this);
        this->setWindowTitle("Set maximum depth");
        buttonOk = new QPushButton(this);
        buttonCancel = new QPushButton(this);
        buttonCancel->setText("Cancel");
        buttonOk->setText("OK");

        connect(buttonCancel, &QPushButton::clicked, this, &QWidget::close);
        connect(box, &QSpinBox::valueChanged, [=](int value)
                { this->controller->maxReadingDepth = value; });
        connect(buttonOk, &QPushButton::clicked, this, &QWidget::close);

        layout->addWidget(label, 0);
        layout->addWidget(box);
        layout->addWidget(buttonCancel);
        layout->addWidget(buttonOk);
        this->setLayout(layout);
    }

private:
    QLabel *label;
    QSpinBox *box;
    QVBoxLayout *layout;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    OfflineFileManagerController* controller;
};

}
