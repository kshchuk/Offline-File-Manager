#include "QFileInfoModel_test.h"

#include <QFileSystemModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QDir>

#include "system_depend_functions.h"


void QFileInfoModel_test::byPathRecursive()
{
    // TODO:
}

void QFileInfoModel_test::readHierarchyRecursive()
{
    // TODO:
}

void QFileInfoModel_test::genFromGoogleDriveResponse()
{
    QByteArray val;
    QFile file;
    file.setFileName("tests/test-response.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
    }
    QJsonDocument response = QJsonDocument::fromJson(val);
    QFileInfoModel model;
    model.genFromGoogleDriveResponse(response);

    QJsonArray arr = response["files"].toArray();
    QJsonValue first = arr[0], second =  arr[1];

    // First
    QCOMPARE(model.index(0, (int)ColunmsOrder::NAME).data().toString(), first["name"].toString());
    QCOMPARE(model.index(0, (int)ColunmsOrder::SIZE).data().toString(), model.fileSize(first["size"].toString().toLongLong()));
    QMimeDatabase dt;
    QCOMPARE(model.index(0, (int)ColunmsOrder::TYPE).data().toString(), dt.mimeTypeForFile(QFileInfo(first["name"].toString())).comment());
    
    QCOMPARE(model.index(0, (int)ColunmsOrder::DATE_MODIDFIED).data().toString(), FromRfc3339(first["modifiedTime"].toString()).toString());
    QCOMPARE(model.index(0, (int)ColunmsOrder::ICON_NAME).data().toString(), first["mimeType"].toString());
    QCOMPARE(model.index(0, (int)ColunmsOrder::DATE_CREATED).data().toString(), FromRfc3339(first["createdTime"].toString()).toString());
    QCOMPARE(model.index(0, (int)ColunmsOrder::GROUP).data().toString(), "");
    // TODO: Finish comparing

    // Second
    QCOMPARE(model.index(1, (int)ColunmsOrder::NAME).data().toString(), second["name"].toString());
    QCOMPARE(model.index(1, (int)ColunmsOrder::SIZE).data().toString(), model.fileSize(second["size"].toString().toLongLong()));
    QCOMPARE(model.index(1, (int)ColunmsOrder::TYPE).data().toString(), dt.mimeTypeForFile(QFileInfo(second["name"].toString())).comment());
                                              
    QCOMPARE(model.index(1, (int)ColunmsOrder::DATE_MODIDFIED).data().toString(), FromRfc3339(second["modifiedTime"].toString()).toString());
    QCOMPARE(model.index(1, (int)ColunmsOrder::ICON_NAME).data().toString(), second["mimeType"].toString());
    QCOMPARE(model.index(1, (int)ColunmsOrder::DATE_CREATED).data().toString(), FromRfc3339(second["createdTime"].toString()).toString());
    QCOMPARE(model.index(1, (int)ColunmsOrder::GROUP).data().toString(), "");
    // TODO: Finish comparing
}

void QFileInfoModel_test::genExternalDrivesModel()
{
    // TODO:
}

void QFileInfoModel_test::genStaticSystemModel()
{
    // TODO:
}

void QFileInfoModel_test::genGoogleDriveModel()
{
    // TODO:
}

void QFileInfoModel_test::insertFileLinkToTheFolder()
{
    // TODO:
}

void QFileInfoModel_test::insertFileToTheFolder()
{
    // TODO:
}

void QFileInfoModel_test::appendFolder()
{
}

void QFileInfoModel_test::deleteFile()
{
    // TODO:
}

void QFileInfoModel_test::setName()
{
    // TODO:
}

void QFileInfoModel_test::getPath()
{
    // TODO:
}

void QFileInfoModel_test::getPathfFromInfo()
{
    // TODO:
}

void QFileInfoModel_test::byPath()
{
    // TODO:
}

void QFileInfoModel_test::pathFromStringList()
{
    // TODO:
}

void QFileInfoModel_test::readFile()
{
    // TODO:
}

void QFileInfoModel_test::writeFile()
{
    // TODO:
}