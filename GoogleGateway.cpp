#include "GoogleGateway.h"

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QString>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QTNetworkAuth/QOAuthHttpServerReplyHandler>
#include <QDesktopServices>


bool GoogleGateway::isAuthorised = false;
QString GoogleGateway::accessToken;


GoogleGateway::GoogleGateway(QObject* parent) : QObject(parent)
{
    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("https://www.googleapis.com/auth/drive.metadata.readonly");

    connect(this->google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](QUrl url) {
        QUrlQuery query(url);

        query.addQueryItem("prompt", "consent");      // Param required to get data everytime
        query.addQueryItem("access_type", "offline"); // Needed for Refresh Token (as AccessToken expires shortly)
        url.setQuery(query);
        QDesktopServices::openUrl(url);
        });

    // Load confs from .json
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
        isAuthorised = true;
        emit authorized();
        });

    connect(this->google, &QOAuth2AuthorizationCodeFlow::tokenChanged,
                     this, [](const QString& token) { accessToken = token; });
}

void GoogleGateway::authorize()
{
    if (!isAuthorised)
        google->grant();
    else {
        google->setToken(accessToken);
        emit authorized();
    }
}

void GoogleGateway::loadFileListPage()
{
    if (!isAuthorised) return;

    QString url = tr("https://www.googleapis.com/drive/v3/files?fields=*");
    if (!nextPageToken.isEmpty()) url += tr("&pageToken=") + nextPageToken;

    auto reply = this->google->get(QUrl(url));
    connect(reply, &QNetworkReply::finished, [=]() {
        qDebug() << "REQUEST FINISHED. Error? " << (reply->error() != QNetworkReply::NoError);
        QJsonDocument* jsonDoc = new QJsonDocument(QJsonDocument::fromJson(reply->readAll()));
        nextPageToken = (*jsonDoc)["nextPageToken"].toString();
        qDebug() << "nextPageToken=" << nextPageToken;

        emit loadedFileListPage(*jsonDoc);

        if (!nextPageToken.isEmpty())
            loadFileListPage();
        });

}