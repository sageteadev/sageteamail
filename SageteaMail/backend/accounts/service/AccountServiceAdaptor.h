/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp /home/dan/dekko/Dekko/backend/accounts/service/dbus/account_service.xml -N -a AccountServiceAdaptor -c AccountServiceAdaptor
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef ACCOUNTSERVICEADAPTOR_H
#define ACCOUNTSERVICEADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.sagetea.AccountService
 */
class AccountServiceAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.sagetea.AccountService")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.sagetea.AccountService\">\n"
"    <signal name=\"accountRemoved\">\n"
"      <arg direction=\"out\" type=\"t\" name=\"id\"/>\n"
"    </signal>\n"
"    <method name=\"removeAccount\">\n"
"      <arg direction=\"in\" type=\"t\" name=\"id\"/>\n"
"    </method>\n"
"    <method name=\"queryAccounts\">\n"
"      <arg direction=\"in\" type=\"ay\" name=\"accountKey\"/>\n"
"      <arg direction=\"in\" type=\"ay\" name=\"sortKey\"/>\n"
"      <arg direction=\"in\" type=\"i\" name=\"limit\"/>\n"
"      <arg direction=\"out\" type=\"(iiii)\" name=\"accounts\"/>\n"
"      <annotation value=\"QList&lt;quint64&gt;\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    AccountServiceAdaptor(QObject *parent);
    virtual ~AccountServiceAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    QList<quint64> queryAccounts(const QByteArray &accountKey, const QByteArray &sortKey, int limit);
    void removeAccount(qulonglong id);
Q_SIGNALS: // SIGNALS
    void accountRemoved(qulonglong id);
};

#endif
