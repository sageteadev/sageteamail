#include "sageteamaildserviceplugin.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QRegExp>
#include <QSettings>
#include <SnapStandardPaths.h>

SageteaMaildService::SageteaMaildService(QObject *parent): ServicePlugin(parent)
{
    m_service = QStringLiteral("sageteamaild");
    m_serviceFile = QString("%1/.config/upstart/%2.conf").arg(QDir::homePath(), m_service);
}

QString SageteaMaildService::pluginId() const
{
    return QStringLiteral("sageteamaild-service");
}

QString SageteaMaildService::location() const
{
    return QStringLiteral("SageteaMail::Service");
}

QString SageteaMaildService::i18n() const
{
    return QString();
}

void SageteaMaildService::start()
{
    if (newVersion()) {
        qDebug() << "[SageteaMaildService] Stopping service for version upgrade";
        stopService();
    }

    if (!serviceRunning()) {
        // The service is started on login, so actually it should already be running.
        // Overwrite service file to make sure thats not causing the problem.
        // see also issue #114
        qDebug() << "[SageteaMaildService] Installing service file";
        installServiceFile();

        qDebug() << "[SageteaMaildService] Starting sageteamaild service";
        startService();
    }
}

void SageteaMaildService::stop()
{
//    if (serviceRunning()) {
//        stopService();
//    }

//    if (serviceFileInstalled()) {
//        removeServiceFile();
//    }
}

bool SageteaMaildService::serviceFileInstalled() const
{
    return QFile(m_serviceFile).exists();
}

bool SageteaMaildService::installServiceFile() const
{
    QFile f(m_serviceFile);
    if (!f.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "[SageteaMaildService] Cannot create service file";
        return false;
    }

    QString appDir = QCoreApplication::applicationDirPath();
    appDir.replace(QRegExp("sageteamail2.sagetea/[^/]+/"), "sageteamail2.sagetea/current/");
    f.write("start on started unity8\n");
    f.write("pre-start script\n");
    f.write("   initctl set-env LD_LIBRARY_PATH=" + appDir.toUtf8() + "/../:$LD_LIBRARY_PATH\n");
    f.write("   initctl set-env SAGETEAMAIL_PLUGINS=" + appDir.toUtf8() + "/../SageteaMail/plugins\n");
    f.write("   initctl set-env QMF_PLUGINS=" + appDir.toUtf8() + "/../qmf/plugins5\n");
    f.write("   initctl set-env QMF_DATA=$HOME/.cache/sageteamail2.sagetea\n");
    f.write("end script\n");
    f.write("exec " + appDir.toUtf8() + "/" + m_service.toUtf8() + "\n");
    f.close();
    return true;
}

bool SageteaMaildService::removeServiceFile() const
{
    if (serviceFileInstalled()) {
        return QFile(m_serviceFile).remove();
    }
    return true;
}

bool SageteaMaildService::serviceRunning() const
{
    QProcess p;
    p.start("initctl", {"status", m_service});
    p.waitForFinished();
    QByteArray output = p.readAll();
    qDebug() << output;
    return output.contains("running");
}

bool SageteaMaildService::startService()
{
    qDebug() << "[SageteaMaildService] should start service";
    int ret = QProcess::execute("start", {m_service});
    return ret == 0;
}

bool SageteaMaildService::restartService()
{
    qDebug() << "[SageteaMaildService] should restart service";
    int ret = QProcess::execute("restart", {m_service});
    return ret == 0;
}

bool SageteaMaildService::stopService()
{
    qDebug() << "[SageteaMaildService] should stop service";
    int ret = QProcess::execute("stop", {m_service});
    return ret == 0;
}

bool SageteaMaildService::newVersion()
{
    static const QString path = SnapStandardPaths::writableLocation(SnapStandardPaths::AppConfigLocation) + QStringLiteral("/sageteamaild/settings.ini");
    QSettings settings(path, QSettings::IniFormat);
    if (!settings.contains(QStringLiteral("version"))) {
        settings.setValue(QStringLiteral("version"), QStringLiteral(SAGETEAMAIL_VERSION));
        // Dekkod may already be running as we previously didn't version it.
        // So we still need to stop it if running.
        return serviceRunning();
    }

    // We also want to support downgrades so just check the version doesn't match DEKKO_VERSION
    const bool result = settings.value(QStringLiteral("version")).toString() != QStringLiteral(SAGETEAMAIL_VERSION);
    if (result) {
        settings.setValue(QStringLiteral("version"), QStringLiteral(SAGETEAMAIL_VERSION));
    }
    settings.sync();
    return result;
}

QVariantMap SageteaMaildService::documentation() const
{
    return QVariantMap();
}

QString SageteaMaildServicePlugin::name() const
{
    return QStringLiteral("sageteamaild-service");
}

QString SageteaMaildServicePlugin::description() const
{
    return QStringLiteral("SageteaMail's messaging server");
}

PluginInfo *SageteaMaildServicePlugin::create(QObject *parent) const
{
    return new SageteaMaildService(parent);
}


