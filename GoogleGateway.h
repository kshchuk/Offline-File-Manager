/**
 * @file GoogleGateway.h
 * @brief Declaration of the GoogleGateway class
 */

#pragma once

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

namespace manager

{
    /**
     * @class GoogleGateway
     * @brief A class that handles authorization and API requests to the Google Drive API
     *
     * This class handles authorization with the Google Drive API using OAuth2 and
     * can be used to make requests to the API to retrieve file lists.
     */
    class GoogleGateway : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructor for GoogleGateway
         * @param parent The parent QObject for this object
         */
        explicit GoogleGateway(QObject *parent = nullptr);

    public slots:
        /**
         * @brief Loads the next page of file list results
         */
        void loadFileListPage();
        /**
         * @brief Begins the authorization flow
         */
        void authorize();

    signals:    
        /**
         * @brief Signal emitted when authorization is complete
         */
        void authorized();
        /**
         * @brief Signal emitted when a file list page is loaded
         * @param doc The QJsonDocument containing the file list data
         */
        void loadedFileListPage(const QJsonDocument &doc);

    private:
        QOAuth2AuthorizationCodeFlow *google; /**< The QOAuth2AuthorizationCodeFlow object used for authorization */

        QString nextPageToken;      /**< The token for the next page of file list results */
        static bool isAuthorised;   /**< Static flag indicating if authorization has been completed */
        static QString accessToken; /**< Static string containing the access token */
    };

}
