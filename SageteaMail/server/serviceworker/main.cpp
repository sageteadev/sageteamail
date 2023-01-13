#include <QCoreApplication>
#include <QDBusConnection>
#include <MailServiceWorker.h>
#include <MailServiceAdaptor.h>
#include <service/AccountServiceWorker.h>
#include <service/AccountServiceAdaptor.h>
#include <qmailnamespace.h>


#define SERVICE "org.sagetea.Service"
#define SERVICE_PATH "/mail"
#define ACCOUNTS_SERVICE "org.sagetea.AccountService"
#define ACCOUNTS_PATH "/accounts"


Q_DECL_EXPORT int main(int argc, char** argv)
{

    QCoreApplication app(argc, argv);

    if(QMail::fileLock("sageteamail-worker.lock") == -1)
        qFatal("Could not get sageteamail worker lock. sageteamail-worker might already be running!");

    MailServiceWorker::registerTypes();

    MailServiceWorker *worker = Q_NULLPTR;
    MailServiceAdaptor *adaptor = Q_NULLPTR;
    AccountServiceWorker *accountsWorker = Q_NULLPTR;
    AccountServiceAdaptor *accountsAdaptor = Q_NULLPTR;

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.interface()->isServiceRegistered(SERVICE)) {
        worker = new MailServiceWorker(&app);
        adaptor = new MailServiceAdaptor(worker);
        Q_UNUSED(adaptor);
        accountsWorker = new AccountServiceWorker(&app);
        accountsAdaptor = new AccountServiceAdaptor(accountsWorker);
        Q_UNUSED(accountsAdaptor);

        if (!connection.registerService(SERVICE)) {
            qFatal("Could not register service");
        }

        if (!connection.registerObject(SERVICE_PATH, worker)) {
            qFatal("Could not register MailServiceWorker object");
        }

        if (!connection.registerObject(ACCOUNTS_PATH, accountsWorker)) {
            qFatal("Could not register AccountServiceWorker object");
        }

    }

    return app.exec();
}
