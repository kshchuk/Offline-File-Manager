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

signals:
    void loadedFileList(QByteArray replylist);

private slots:
    void loadFileList();

private:
    QOAuth2AuthorizationCodeFlow* google;

    static bool isAuthorised;
    static QString accessToken;
};
