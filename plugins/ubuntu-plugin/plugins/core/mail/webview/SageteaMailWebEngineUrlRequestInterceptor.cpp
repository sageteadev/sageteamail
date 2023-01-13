#include "SageteaMailWebEngineUrlRequestInterceptor.h"
#include <QWebEngineUrlRequestInfo>
#include <QDebug>

SageteaMailWebEngineUrlRequestInterceptor::SageteaMailWebEngineUrlRequestInterceptor(QWebEngineUrlRequestInterceptor *parent) : QWebEngineUrlRequestInterceptor(parent),
    remoteResourcesAreBlocked(true)
{
}

void SageteaMailWebEngineUrlRequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    auto requestUrl = info.requestUrl();

    if (!this->hasAllowedScheme(requestUrl))
    {
        // check the resourceType() to not block links, not sure thats reliable, but it
        // seems navigationType() == QWebEngineUrlRequestInfo::NavigationTypeLink always...
        bool doBlock = this->remoteResourcesAreBlocked
                       && (info.resourceType() != QWebEngineUrlRequestInfo::ResourceTypeMainFrame);
        info.block(doBlock);
        emit interceptedRemoteRequest(doBlock);
    }
}

bool SageteaMailWebEngineUrlRequestInterceptor::hasAllowedScheme(const QUrl &url)
{
    return this->testScheme("cid", url) || this->testScheme("dekko-part", url) || this->testScheme("dekko-msg", url) || this->testScheme("blob", url);
}

bool SageteaMailWebEngineUrlRequestInterceptor::testScheme(const QString &scheme, const QUrl &url)
{
    return url.scheme() == scheme;
}

void SageteaMailWebEngineUrlRequestInterceptor::setBlockRemoteResources(bool doBlock)
{
    this->remoteResourcesAreBlocked = doBlock;
}

bool SageteaMailWebEngineUrlRequestInterceptor::areRemoteResourcesBlocked() const
{
    return this->remoteResourcesAreBlocked;
}

