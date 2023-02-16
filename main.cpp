#include "OfflineFileManager.h"
#include <QtWidgets/QApplication>

#include "Searcher_test.h"
#include "ModelSerializer_test.h"
#include "QFileInfoModel_test.h"
#include "PropertiesLogic_test.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QFile styleFile("res/css/Combinear.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);
    OfflineFileManager w;

#ifdef _DEBUG
    freopen("testing.log", "w", stdout);
    QTest::qExec(new ModelSerializer_test, argc, argv);
    QTest::qExec(new Searcher_test, argc, argv);
    QTest::qExec(new QFileInfoModel_test, argc, argv);
    QTest::qExec(new PropertiesLogic_test, argc, argv);
#endif

    w.show();
    return a.exec();
}
