#ifndef SAGETEAMAILWEBCONTEXT_H
#define SAGETEAMAILWEBCONTEXT_H

#include "SageteaMailWebEngineUrlRequestInterceptor.h"
#include "SageteaMailWebEngineUrlSchemeHandler.h"
#include <QString>
#include <QUrl>
#include <QQuickWebEngineProfile>

class SageteaMailWebContext : public QQuickWebEngineProfile
{
    Q_OBJECT
    Q_PROPERTY(QString messageUid READ getMessageUid WRITE setMessageUid NOTIFY messageUidChanged)
    Q_PROPERTY(bool remoteContentAllowed READ isRemoteContentAllowed WRITE setRemoteContentAllowed NOTIFY remoteContentAllowedChanged)
    Q_PROPERTY(QString cacheLocation READ cacheLocation NOTIFY cacheLocationChanged)
    Q_PROPERTY(QString dataLocation READ dataLocation NOTIFY dataLocationChanged)
    Q_PROPERTY(int cacheSizeHint READ cacheSizeHint NOTIFY cacheSizeHintChanged)
    Q_PROPERTY(QObject * qmlEngineInjector READ qmlEngineInjector WRITE setQmlEngineInjector NOTIFY qmlEngineInjectorChanged)

public:
    explicit SageteaMailWebContext(QQuickWebEngineProfile *parent = Q_NULLPTR);
    ~SageteaMailWebContext() {
    }

    void setMessageUid(QString messageUid);
    void setRemoteContentAllowed(bool allowed);
    QString getMessageUid() const;
    bool isRemoteContentAllowed() const;

    QString cacheLocation() const;
    QString dataLocation() const;
    int cacheSizeHint() const;

    void setQmlEngineInjector(QObject * dummy);
    QObject * qmlEngineInjector() const;

signals:
    void remoteContentBlocked();
    void messageUidChanged();
    void remoteContentAllowedChanged();

    void cacheLocationChanged();
    void dataLocationChanged();
    void cacheSizeHintChanged();  
    void qmlEngineInjectorChanged();

public slots:
    void onInterceptedRemoteRequest(bool wasBlocked);

private:
    SageteaMailWebEngineUrlRequestInterceptor urlRequestInterceptor;
    SageteaMailWebEngineUrlSchemeHandler urlSchemeHandler;
};

#endif
