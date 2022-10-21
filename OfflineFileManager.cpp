#include "OfflineFileManager.h"
#include <QCommandLineParser>
#include <QFileIconProvider>
#include <QScreen>
#include <Qscroller>

#include <QObject>


OfflineFileManager::OfflineFileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    model.setRootPath("");

    ui.addressLine->setText("/");

    ui.fileSystemModelView->setModel(&model);
    ui.fileSystemModelView->setAnimated(false);
    ui.fileSystemModelView->setIndentation(20);
    ui.fileSystemModelView->setSortingEnabled(true);

    const QSize availableSize = ui.fileSystemModelView->screen()->availableGeometry().size();
    ui.fileSystemModelView->setColumnWidth(0, ui.fileSystemModelView->width() / 2);

    QScroller::grabGesture(ui.fileSystemModelView, QScroller::TouchGesture);

    // QObject::connect()
}

OfflineFileManager::~OfflineFileManager()
{}

void OfflineFileManager::set_AddresLineText(const QModelIndex &element)
{
    ui.addressLine->setText(model.filePath(element));
}
