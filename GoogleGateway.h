#pragma once

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

namespace manager {
class GoogleGateway : public QObject
{
    Q_OBJECT
public:
    explicit GoogleGateway(QObject* parent = nullptr);

public slots:
    void loadFileListPage();
    void authorize();

signals:
    void authorized();
    void loadedFileListPage(const QJsonDocument& doc);

private:
    QOAuth2AuthorizationCodeFlow* google;

    QString nextPageToken;
    static bool isAuthorised;
    static QString accessToken;
};

}
