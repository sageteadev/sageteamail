#include "SageteaMailWebContextPlugin.h"
#include "SageteaMailWebContext.h"
#include <QtQml/QtQml>
#include <QtQml/QQmlContext>

void SageteaMailWebContextPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("SageteaMail.WebContext"));
    // @uri SageteaMail.WebContext
    qmlRegisterType<SageteaMailWebContext>(uri, 1, 0, "SageteaMailWebContext");
}

void SageteaMailWebContextPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
