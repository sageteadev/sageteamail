#ifndef SAGETEAMAILWEBENGINEURLREQUESTINTERCEPTOR_H
#define SAGETEAMAILWEBENGINEURLREQUESTINTERCEPTOR_H

#include <QString>
#include <QUrl>
#include <QWebEngineUrlRequestInterceptor>

class SageteaMailWebEngineUrlRequestInterceptor : public QWebEngineUrlRequestInterceptor
{

    Q_OBJECT
public:
    void interceptRequest(QWebEngineUrlRequestInfo &info);
    explicit SageteaMailWebEngineUrlRequestInterceptor(QWebEngineUrlRequestInterceptor *parent = Q_NULLPTR);
    ~SageteaMailWebEngineUrlRequestInterceptor() {
    }

//    void setCid(QString cid);
    void setBlockRemoteResources(bool doBlock);
//    QString getCid() const;
    bool areRemoteResourcesBlocked() const;

signals:
    void interceptedRemoteRequest(bool wasBlocked);

private:
    bool testScheme(const QString &scheme, const QUrl &url);
    bool hasAllowedScheme(const QUrl &url);
    bool remoteResourcesAreBlocked;
//    QString cid;
};

#endif
