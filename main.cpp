#include "OfflineFileManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OfflineFileManager w;
    w.show();
    return a.exec();
}
