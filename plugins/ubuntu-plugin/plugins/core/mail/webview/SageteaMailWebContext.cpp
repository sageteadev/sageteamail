#include "SageteaMailWebContext.h"
#include "SageteaMailWebEngineUrlSchemeHandler.h"
#include "SageteaMailWebEngineUrlRequestInterceptor.h"
#include <QQmlEngine>
#include <QtQml>
#include <QDebug>
#include <QDir>
#include <QStorageInfo>
#include <QCoreApplication>
#include <QStandardPaths>

SageteaMailWebContext::SageteaMailWebContext(QQuickWebEngineProfile *parent) : QQuickWebEngineProfile(parent)
{
    this->setRequestInterceptor(&this->urlRequestInterceptor);

    this->installUrlSchemeHandler("dekko-msg", &this->urlSchemeHandler);
    this->installUrlSchemeHandler("dekko-part", &this->urlSchemeHandler);
    this->installUrlSchemeHandler("cid", &this->urlSchemeHandler);
    connect(&this->urlRequestInterceptor, SIGNAL(interceptedRemoteRequest(bool)), this, SLOT(onInterceptedRemoteRequest(bool)));
}

void SageteaMailWebContext::onInterceptedRemoteRequest(bool wasBlocked)
{
    if (wasBlocked)
    {
        emit remoteContentBlocked();
    }
}

void SageteaMailWebContext::setMessageUid(QString messageUid)
{
    this->urlSchemeHandler.setMessageUid(messageUid);
}

void SageteaMailWebContext::setRemoteContentAllowed(bool allowed)
{
    this->urlRequestInterceptor.setBlockRemoteResources(!allowed);
}

QString SageteaMailWebContext::getMessageUid() const
{
    return this->urlSchemeHandler.getMessageUid();
}

bool SageteaMailWebContext::isRemoteContentAllowed() const
{
    return !this->urlRequestInterceptor.areRemoteResourcesBlocked();
}


QString SageteaMailWebContext::cacheLocation() const
{
    QDir location(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!location.exists()) {
        QDir::root().mkpath(location.absolutePath());
    }
    return location.absolutePath();
}

QString SageteaMailWebContext::dataLocation() const
{
    QDir location(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if (!location.exists()) {
        QDir::root().mkpath(location.absolutePath());
    } else {
        // Prior to fixing https://launchpad.net/bugs/1424726, chromium’s cache
        // data was written to the data location. Purge the old cache data.
        QDir(location.absoluteFilePath("Cache")).removeRecursively();
    }
    return location.absolutePath();
}

int SageteaMailWebContext::cacheSizeHint() const
{
    if (QCoreApplication::applicationName() == "webbrowser-app") {
        // Let chromium decide the optimum cache size based on available disk space
        return 0;
    } else {
        // For webapps and other embedders, determine the cache size hint
        // using heuristics based on the disk space (total, and available).
        QStorageInfo storageInfo(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        const int MB = 1024 * 1024;
        // The total cache size for all apps should not exceed 10% of the total disk space
        int maxSharedCache = storageInfo.bytesTotal() / MB * 0.1;
        // One given app is allowed to use up to 5% o the total cache size
        int maxAppCacheAllowance = maxSharedCache * 0.05;
        // Ensure it never exceeds 200 MB though
        int maxAppCacheAbsolute = qMin(maxAppCacheAllowance, 200);
        // Never use more than 20% of the available disk space
        int maxAppCacheRelative = storageInfo.bytesAvailable() / MB * 0.2;
        // Never set a size hint below 5 MB, as that would result in a very inefficient cache
        return qMax(5, qMin(maxAppCacheAbsolute, maxAppCacheRelative));
    }
}

QObject * SageteaMailWebContext::qmlEngineInjector() const
{
    return nullptr;
}

void SageteaMailWebContext::setQmlEngineInjector(QObject * dummy)
{
    QQmlEngine* engine = qmlEngine(dummy);
    if (!engine) {
        qDebug() << "Failed to get qml engine from injected qml object.";
        return;
    }

    this->urlSchemeHandler.setNetworkAccessManager(engine->networkAccessManager());
}
