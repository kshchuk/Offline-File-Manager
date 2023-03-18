#include "QFileInfoModel.h"
#include "OfflineFileManagerController.h"
#include "OfflineFileManagerView.h"

#include <QtWidgets/QApplication>

//#include "Searcher_test.h"
#include "tests/ModelSerializer_test.hpp"
//#include "QFileInfoModel_test.h"
//#include "PropertiesLogic_test.h"


int main(int argc, char *argv[])
{
    freopen("testing.log", "w", stdout);
    QTest::qExec(new ModelSerializer_test, argc, argv);
    //QTest::qExec(new Searcher_test, argc, argv);
    //QTest::qExec(new QFileInfoModel_test, argc, argv);
    //QTest::qExec(new PropertiesLogic_test, argc, argv);

    QApplication a(argc, argv);
    manager::OfflineFileManagerView w;
    QFile styleFile("res/css/Combinear.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    w.setStyleSheet(style);
    a.setStyleSheet(style);

    auto m = new manager::ExternalDrivesModel();
    manager::OfflineFileManagerController c(m, &w);
    w.setModel(m);
    w.show();
    return a.exec();
}
