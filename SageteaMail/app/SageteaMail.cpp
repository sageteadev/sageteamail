/* Copyright (C) 2016 - 2017 Dan Chapman <dpniel@ubuntu.com>

   This file is part of SageteaMail email client for Ubuntu devices

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "SageteaMail.h"
#include <QDebug>
#include <qmailnamespace.h>
#include <qmaillog.h>
#include <QUrl>
#include <QDirIterator>
#include <QtQuick/QQuickView>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QCommandLineOption>
#include <QDir>
#include <SnapStandardPaths.h>
#include <PluginRegistry.h>

#define SMALL_FF_WIDTH 350
#define MEDIUM_FF_WDTH 800
#define LARGE_FF_WIDTH 1100

Q_LOGGING_CATEGORY(SAGETEAMAIL_MAIN, "sageteamail.main")

SageteaMail::SageteaMail(int &argc, char **argv) :
    QApplication(argc, argv),
    m_serviceRegistry(0),
#ifdef SERVER_AS_QTHREAD
    m_serverThread(0),
#else
    m_server(0),
    m_worker(0),
#endif
    devMode(false),
    m_verboseLogging(false)
{
    QCoreApplication::setOrganizationName(APP_ORG);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(SAGETEAMAIL_VERSION);

    // Uncomment to dump out the resource files
    // Useful to be able to check a resource has been included
//    QDirIterator it(":", QDirIterator::Subdirectories);
//    while (it.hasNext()) {
//        qCDebug(SAGETEAMAIL_MAIN) << it.next();
//    }
    parser.setApplicationDescription("SageteaMail email client");
    parser.addHelpOption();
    parser.addOption({"d", "Enable dev mode"});
    parser.addOption({"v", "Verbose logging"});
    parser.addOption({"small", "Open in small form factor state"});
    parser.addOption({"medium", "Open in medium form factor state"});
    parser.addOption({"large", "Open in large form factor state"});
    parser.addOption({"contacts", "Open the addressbook"});

    QStringList cArgs = this->arguments();

    for (const QString &s : cArgs) {
        if (s.startsWith("--desktop_file_hint=")) {
            cArgs.removeAll(s);
        }
    }

    parser.process(cArgs);

    m_verboseLogging = (parser.isSet("d") || parser.isSet("v") || QFile::exists(QStringLiteral("/tmp/sageteamail-debug")));

    if(!m_verboseLogging)
            qputenv("QT_LOGGING_RULES", "sageteamail.*=false");
        else
            qputenv("QT_LOGGING_RULES", "sageteamail.*=true");

}

SageteaMail::~SageteaMail(){
#ifndef SERVER_AS_QTHREAD
    delete m_server;
    m_server = 0;
    delete m_worker;
    m_worker = 0;
#endif
}

bool SageteaMail::setup()
{
    QStringList arguments = this->arguments();
    Q_UNUSED(arguments);

    if (!QMail::mkLockDir()) {
        return false;
    }

    if (qgetenv("QMF_DATA").isEmpty()) {
        // Fall back to standard xdg cache location
        qputenv("QMF_DATA", SnapStandardPaths::writableLocation(SnapStandardPaths::AppCacheLocation).toUtf8());
    }


    loadPlugins();
    m_serviceRegistry = new ServiceRegistry(this);
    m_serviceRegistry->setServiceKey(QStringLiteral("SageteaMail::Service"));

#if defined(CLICK_MODE)
    m_serviceRegistry->startServices();
#else
    if (!isServerRunning()) {
        qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message server not running attempting to start";
        if (!startServer()) {
            qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message server failed to start";
            return false;
        } else {
            qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message server started successfully \\o/";
        }
    } else {
        qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message server already running, using that";
    }
    m_serviceRegistry->startServices();
#endif

    if (!isWorkerRunning()) {
        qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message worker not running attempting to start";
        if (!startWorker()) {
            qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message worker failed to start";
            return false;
        } else {
            qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message worker started successfully \\o/";
        }
    } else {
        qCDebug(SAGETEAMAIL_MAIN) << "[SageteaMail]" << "Message worker already running, using that";
    }
    m_engine.setNetworkAccessManagerFactory(&m_partqnam);

    devMode = parser.isSet("d");

    m_engine.rootContext()->setContextProperty("sageteamailapp", this);
    m_engine.rootContext()->setContextProperty("service", m_serviceRegistry);
    m_engine.rootContext()->setContextProperty("devModeEnabled", QVariant(devMode));
    m_engine.rootContext()->setContextProperty("verboseLogging", QVariant(m_verboseLogging));
    // Context property to figure out if we are on unity8/mir or not
    m_engine.rootContext()->setContextProperty(QStringLiteral("isRunningOnMir"), QVariant(qgetenv("QT_QPA_PLATFORM") == "ubuntumirclient"));
    // Check both APP_URIS and positionalArgs for uris we need to handle
    QStringList uris;
    uris << qgetenv("APP_URIS");
    uris << parser.positionalArguments();
    if (parser.isSet("contacts")) {
        uris << QStringLiteral("sageteamail://contacts/open");
    }
    uris.removeDuplicates();
    m_engine.rootContext()->setContextProperty(QStringLiteral("appUris"), uris);
    m_engine.load(QUrl("qrc:/qml/SageteaMail.qml"));
    return true;
}

bool SageteaMail::isServerRunning()
{
    int lockid = QMail::fileLock(QStringLiteral("sageteamaild-instance.lock"));
    if (lockid == -1)
        return true;

    QMail::fileUnlock(lockid);
    return false;
}

bool SageteaMail::startServer()
{
#ifdef SERVER_AS_QTHREAD
    // Use MessageServerThread
    m_serverThread = new MessageServerThread();
    m_serverThread->start();
    QEventLoop loop;
    QObject::connect(m_serverThread, &MessageServerThread::messageServerStarted, &loop, &QEventLoop::quit);
    loop.exec();
    return true;
#else
    if (m_server) {
        delete m_server;
        m_server = 0;
    }
    m_server = new QProcess(this);
    static const QString binary(QString("/sageteamaild"));
    connect(m_server,SIGNAL(error(QProcess::ProcessError)),
            this,SLOT(serverProcessError(QProcess::ProcessError)));
    m_server->start(QMail::messageServerPath() + binary);
    return m_server->waitForStarted();
#endif
}

bool SageteaMail::isWorkerRunning()
{
    int lockid = QMail::fileLock(QStringLiteral("sageteamail-worker.lock"));
    if (lockid == -1)
        return true;

    QMail::fileUnlock(lockid);
    return false;
}

bool SageteaMail::startWorker()
{
    if (m_worker) {
        delete m_worker;
        m_worker = 0;
    }
    m_worker = new QProcess(this);
    static const QString binary(QString("/sageteamail-worker"));
    connect(m_worker,SIGNAL(error(QProcess::ProcessError)),
            this,SLOT(workerProcessError(QProcess::ProcessError)));
    connect(m_worker, &QProcess::readyRead, [=](){ if (m_worker->canReadLine()) qCDebug(SAGETEAMAIL_MAIN) << m_worker->readLine(); });
    m_worker->start(QMail::messageServerPath() + binary);
    return m_worker->waitForStarted();
}

void SageteaMail::trimCache()
{
    m_engine.trimComponentCache();
}
// TODO: show popup in mainview about server vanishing and SageteaMail will now close.
void SageteaMail::serverProcessError(QProcess::ProcessError error)
{
    qDebug() << "[Error] " << error;
    qWarning() << "[ERROR] Message server stopped, trying to restart";
    delete m_server;
    m_server = 0;
    startServer();
}

void SageteaMail::workerProcessError(QProcess::ProcessError error)
{
    qDebug() << "[Error] " << error;
    qWarning() << "[ERROR] Message worker stopped, trying to restart";
    delete m_worker;
    m_worker = 0;
    startWorker();
}

void SageteaMail::loadPlugins()
{
    PluginRegistry::instance()->setPluginLocations(
                QStringList()
                << QStringLiteral("SageteaMail::Mail")
                << QStringLiteral("SageteaMail::Mail::Composer")
                << QStringLiteral("SageteaMail::Mail::Composer::UI")
                << QStringLiteral("SageteaMail::Mail::Settings")
                << QStringLiteral("SageteaMail::Mail::SetupWizard")
                << QStringLiteral("SageteaMail::Stage::Settings")
                << QStringLiteral("SageteaMail::Settings::AccountAction")
                << QStringLiteral("SageteaMail::Settings::PluginAction")
                << QStringLiteral("SageteaMail::Settings::MailAction")
                << QStringLiteral("SageteaMail::Stage::Main")
                << QStringLiteral("SageteaMail::Stage::Mail")
                << QStringLiteral("SageteaMail::Stage::Action")
                << QStringLiteral("SageteaMail::Mail::NavHeadAction")
                << QStringLiteral("SageteaMail::Service")
                << QStringLiteral("SageteaMail::Listener")
                << QStringLiteral("SageteaMail::AddressBook::Provider")
                << QStringLiteral("SageteaMail::Mail::MultiSelectAction")
                << QStringLiteral("SageteaMail::Mail::FolderPicker")
                );

   PluginRegistry::instance()->loadPlugins(
               QStringList()
               << QStringLiteral("%1/plugins").arg(SnapStandardPaths::writableLocation(SnapStandardPaths::AppDataLocation))
               );
}
