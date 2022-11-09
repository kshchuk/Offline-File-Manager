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

public slots:
    void loadFileList();

private:
    QOAuth2AuthorizationCodeFlow* google;

    QNetworkReply* reply;
};
