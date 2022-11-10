#pragma once

#include <QObject>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

class GoogleGateway : public QObject
{
    Q_OBJECT
public:
    explicit GoogleGateway(QObject* parent = nullptr);

private slots:
    void loadFileList();
    void treeGot(QNetworkReply* reply);

private:
    QOAuth2AuthorizationCodeFlow* google;
};
