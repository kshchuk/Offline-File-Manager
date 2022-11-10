#include "GoogleGateway.h"

#include "googlegateway.h"
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QTNetworkAuth/QOAuthHttpServerReplyHandler>
#include <QDesktopServices>



GoogleGateway::GoogleGateway(QObject* parent) : QObject(parent)
{
    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("https://www.googleapis.com/auth/drive.metadata.readonly");

    connect(this->google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    QByteArray val;
    QFile file;
    file.setFileName(QDir::toNativeSeparators("client.json"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
    }

    QJsonDocument document = QJsonDocument::fromJson(val);
    const auto object = document.object();
    const auto settingsObject = object["web"].toObject();
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret(settingsObject["client_secret"].toString());
    const auto redirectUris = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUri(redirectUris[0].toString()); // Get the first URI
    const auto port = static_cast<quint16>(redirectUri.port()); // Get the port

    google->setAuthorizationUrl(authUri);
    google->setClientIdentifier(clientId);
    google->setAccessTokenUrl(tokenUri);
    google->setClientIdentifierSharedKey(clientSecret);

    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    this->google->setReplyHandler(replyHandler);

    connect(this->google, &QOAuth2AuthorizationCodeFlow::granted, [=]() {
        qDebug() << __FUNCTION__ << __LINE__ << "Access Granted!";

        auto reply = this->google->get(QUrl("https://www.googleapis.com/plus/v1/people/me"));
        connect(reply, &QNetworkReply::finished, [reply]() {
            qDebug() << "REQUEST FINISHED. Error? " << (reply->error() != QNetworkReply::NoError);
            qDebug() << reply->readAll();
            });
        connect(reply, &QNetworkReply::finished, this, &GoogleGateway::loadFileList);
        });

    this->google->grant();

    loadFileList();
}

void GoogleGateway::loadFileList()
{
    QString token = google->token();
    QNetworkRequest request;
    request.setRawHeader(QByteArray("Authorization"), token.toLatin1());
    QUrl url("https://www.googleapis.com/drive/v3/files?fields=*");
    request.setUrl(url);

    auto m_netM = new QNetworkAccessManager;

    connect(m_netM, &QNetworkAccessManager::finished, this, &GoogleGateway::treeGot);

    m_netM->get(request);
    m_netM->deleteLater();
}

void GoogleGateway::treeGot(QNetworkReply* reply)
{
    QString json = reply->readAll();
}


