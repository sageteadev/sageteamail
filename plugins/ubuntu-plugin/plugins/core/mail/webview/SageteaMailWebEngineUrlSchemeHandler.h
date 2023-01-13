#ifndef SAGETEAMAILURLINTERCEPTOR_H
#define SAGETEAMAILURLINTERCEPTOR_H

#include <QString>
#include <QUrl>
#include <QWebEngineUrlSchemeHandler>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebEngineUrlRequestJob>

class SageteaMailWebEngineUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    void requestStarted(QWebEngineUrlRequestJob *request);
    explicit SageteaMailWebEngineUrlSchemeHandler(QWebEngineUrlSchemeHandler *parent = Q_NULLPTR);
    ~SageteaMailWebEngineUrlSchemeHandler() {
    }

    void setMessageUid(QString messageUid);
    QString getMessageUid() const;
    void setNetworkAccessManager(QNetworkAccessManager * qnam);
    
public slots:
    void onReply(QNetworkReply *reply);

private:
    bool testScheme(const QString &scheme, const QUrl &url);
    QString messageUid;
    QWebEngineUrlRequestJob * pendingRequest;
    QNetworkAccessManager * customNetworkAccessManager;
};

#endif
