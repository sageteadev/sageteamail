#include "SageteaMailWebEngineUrlSchemeHandler.h"
#include <QDebug>
#include <QNetworkRequest>

SageteaMailWebEngineUrlSchemeHandler::SageteaMailWebEngineUrlSchemeHandler(QWebEngineUrlSchemeHandler *parent) : QWebEngineUrlSchemeHandler(parent),
    messageUid(""), pendingRequest(nullptr), customNetworkAccessManager(nullptr)
{
}

void SageteaMailWebEngineUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request)
{
    auto requestUrl = request->requestUrl();
    qDebug() << "Scheme handler called for " << requestUrl.toString();
    if (this->testScheme("cid", requestUrl) && !requestUrl.toString().contains(this->messageUid))
    {
        requestUrl.setQuery("messageId=" + this->messageUid);
    }

    pendingRequest = request;

    QNetworkRequest networkRequest;
    networkRequest.setUrl(requestUrl);

    if (!this->customNetworkAccessManager)
    {
        qDebug() << "I can get no network access manager...";
    }
    else
    {
        this->customNetworkAccessManager->get(networkRequest);
    }
}

void SageteaMailWebEngineUrlSchemeHandler::onReply(QNetworkReply *reply)
{
    if (!pendingRequest)
    {
        return;
    }
    connect(this->pendingRequest, &QObject::destroyed, reply, &QObject::deleteLater);
    this->pendingRequest->reply(QByteArrayLiteral("text/html"), reply);
    this->pendingRequest = nullptr;
}

bool SageteaMailWebEngineUrlSchemeHandler::testScheme(const QString &scheme, const QUrl &url)
{
    return url.scheme() == scheme;
}

void SageteaMailWebEngineUrlSchemeHandler::setMessageUid(QString messageUid)
{
    this->messageUid = messageUid;
}

QString SageteaMailWebEngineUrlSchemeHandler::getMessageUid() const
{
    return this->messageUid;
}

void SageteaMailWebEngineUrlSchemeHandler::setNetworkAccessManager(QNetworkAccessManager * qnam)
{
    this->customNetworkAccessManager = qnam;

    connect(this->customNetworkAccessManager, &QNetworkAccessManager::finished,
            this, &SageteaMailWebEngineUrlSchemeHandler::onReply);
}

