#include "notifyserviceplugin.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QRegExp>
#include <QSettings>
#include <SnapStandardPaths.h>


NotifyService::NotifyService(QObject *parent): ServicePlugin(parent)
{
    m_service = QStringLiteral("sageteamaild-notify");
    m_serviceFile = QString("%1/.config/upstart/%2.conf").arg(QDir::homePath(), m_service);
}

QString NotifyService::pluginId() const
{
    return QStringLiteral("sageteamaild-notify-service");
}

QString NotifyService::location() const
{
    return QStringLiteral("SageteaMail::Service");
}

QString NotifyService::i18n() const
{
    return QString();
}

void NotifyService::start()
{

    if (newVersion() && serviceRunning()) {
        qDebug() << "[NotifyService] Stopping service for version upgrade";
        stopService();
    }

    if (!serviceRunning()) {
        // The service is started on login, so actually it should already be running.
        // Overwrite service file to make sure thats not causing the problem.
        // see also issue #114
        qDebug() << "[NotifyService] Installing service file";
        installServiceFile();

        qDebug() << "[NotifyService] Starting sageteamaild-notify service";
        startService();
    }
}

void NotifyService::stop()
{
//    if (serviceRunning()) {
//        stopService();
//    }

//    if (serviceFileInstalled()) {
//        removeServiceFile();
//    }
}

bool NotifyService::serviceFileInstalled() const
{
    return QFile(m_serviceFile).exists();
}

bool NotifyService::newVersion()
{
    static const QString path = SnapStandardPaths::writableLocation(SnapStandardPaths::AppConfigLocation) + QStringLiteral("/sageteamaild-notify/settings.ini");
    QSettings settings(path, QSettings::IniFormat);
    if (!settings.contains(QStringLiteral("version"))) {
        settings.setValue(QStringLiteral("version"), QStringLiteral(SAGETEAMAIL_VERSION));
        return serviceRunning();
    }

    // We also want to support downgrades so just check the version doesn't match SAGETEAMAIL_VERSION
    const bool result = settings.value(QStringLiteral("version")).toString() != QStringLiteral(SAGETEAMAIL_VERSION);
    if (result) {
        settings.setValue(QStringLiteral("version"), QStringLiteral(SAGETEAMAIL_VERSION));
    }
    settings.sync();
    return result;
}

bool NotifyService::installServiceFile() const
{
    QFile f(m_serviceFile);
    if (!f.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "[NotifyService] Cannot create service file";
        return false;
    }

    QString appDir = QCoreApplication::applicationDirPath();
    appDir.replace(QRegExp("sageteamail2.sagetea/[^/]+/"), "sageteamail2.sagetea/current/");
    f.write("start on started sageteamaild\n");
    f.write("pre-start script\n");
    f.write("   initctl set-env LD_LIBRARY_PATH=" + appDir.toUtf8() + "/../:$LD_LIBRARY_PATH\n");
    f.write("   initctl set-env SAGETEAMAIL_PLUGINS=" + appDir.toUtf8() + "/../SageteaMail/plugins\n");
    f.write("   initctl set-env QMF_PLUGINS=" + appDir.toUtf8() + "/../qmf/plugins5\n");
    f.write("   initctl set-env QMF_DATA=$HOME/.cache/sageteamail2.sagetea\n");
    f.write("end script\n");
    f.write("exec " + appDir.toUtf8() + "/plugins/notify/" + m_service.toUtf8() + "\n");
    f.close();
    return true;
}

bool NotifyService::removeServiceFile() const
{
    if (serviceFileInstalled()) {
        return QFile(m_serviceFile).remove();
    }
    return true;
}

bool NotifyService::serviceRunning() const
{
    QProcess p;
    p.start("initctl", {"status", m_service});
    p.waitForFinished();
    QByteArray output = p.readAll();
    qDebug() << output;
    return output.contains("running");
}

bool NotifyService::startService()
{
    qDebug() << "[NotifyService] should start service";
    int ret = QProcess::execute("start", {m_service});
    return ret == 0;
}

bool NotifyService::restartService()
{
    qDebug() << "[NotifyService] should restart service";
    int ret = QProcess::execute("restart", {m_service});
    return ret == 0;
}

bool NotifyService::stopService()
{
    qDebug() << "[NotifyService] should stop service";
    int ret = QProcess::execute("stop", {m_service});
    return ret == 0;
}

QVariantMap NotifyService::documentation() const
{
    return QVariantMap();
}

QString NotifyServicePlugin::name() const
{
    return QStringLiteral("sageteamaild-notify-service");
}

QString NotifyServicePlugin::description() const
{
    return QStringLiteral("SageteaMail notification service");
}

PluginInfo *NotifyServicePlugin::create(QObject *parent) const
{
    return new NotifyService(parent);
}
