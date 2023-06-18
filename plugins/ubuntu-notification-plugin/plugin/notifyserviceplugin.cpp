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
    m_serviceFile = QString("%1/.config/systemd/user/%2.service").arg(QDir::homePath(), m_service);
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

bool NotifyService::installServiceFile() const
{
    QFile f(m_serviceFile);
    QDir parent = QFileInfo(f).dir();

    if (!parent.mkpath(".")) {
        qDebug() << "[NotifyService] Cannot create systemd user service directory";
        return false;
    }

    if (!f.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "[NotifyService] Cannot create service file";
        return false;
    }

    QString appDir = QCoreApplication::applicationDirPath();
    appDir.replace(QRegExp("sageteamail2.sagetea/[^/]+/"), "sageteamail2.sagetea/current/");

    f.write("[Unit]\n");
    f.write("Description=SageteaMail Notify Service\n");
    f.write("Requires=sageteamaild.service\n");

    f.write("\n[Service]\n");
    f.write("ExecStart=" + appDir.toUtf8() + "/plugins/notify/" + m_service.toUtf8() + "\n");
    f.write("WorkingDirectory=" + appDir.toUtf8() + "\n");
    f.write("Environment=\"LD_LIBRARY_PATH=" + appDir.toUtf8() + "/../:$LD_LIBRARY_PATH\"\n");
    f.write("Environment=\"SAGETEAMAIL_PLUGINS=" + appDir.toUtf8() + "/../SageteaMail/plugins\"\n");
    f.write("Environment=\"QMF_PLUGINS=" + appDir.toUtf8() + "/../qmf/plugins5\"\n");
    f.write("Environment=\"QMF_DATA=" + QDir::homePath().toUtf8() + "/.cache/sageteamail2.sagetea\"\n");
    f.write("Restart=on-failure\n");

    f.write("\n[Install]\n");
    f.write("WantedBy=graphical-session.target\n");
    
    f.close();

    qDebug() << "[NotifyService] should enable service";
    int ret1 = QProcess::execute("systemctl", {"--user", "daemon-reload"});
    int ret2 = QProcess::execute("systemctl", {"--user", "enable", m_service});

    return ret1 == 0 && ret2 == 0;
}

bool NotifyService::removeServiceFile() const
{
    if (serviceFileInstalled()) {
        int ret = QProcess::execute("systemctl", {"--user", "disable", "--now", m_service});
        return QFile(m_serviceFile).remove() && ret == 0;
    }
    return true;
}

bool NotifyService::serviceRunning() const
{
    QProcess p;
    p.start("systemctl", {"--user", "status", m_service});
    p.waitForFinished();
    QByteArray output = p.readAll();
    qDebug() << output;
    return output.contains("active (running)");
}

bool NotifyService::startService()
{
    qDebug() << "[NotifyService] should start service";
    int ret = QProcess::execute("systemctl", {"--user", "start", m_service});
    return ret == 0;
}

bool NotifyService::restartService()
{
    qDebug() << "[NotifyService] should restart service";
    int ret = QProcess::execute("systemctl", {"--user", "restart", m_service});
    return ret == 0;
}

bool NotifyService::stopService()
{
    qDebug() << "[NotifyService] should stop service";
    int ret = QProcess::execute("systemctl", {"--user", "stop", m_service});
    return ret == 0;
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
