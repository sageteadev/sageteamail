/* Copyright (C) 2015 Dan Chapman <dpniel@ubuntu.com>

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
#include "emailvalidator.h"
#include <QDebug>
#include <QRegularExpression>

EmailValidator::EmailValidator(QObject *parent) :
    QObject(parent)
{
}

bool EmailValidator::validate(const QString &emailAddress)
{
    QString rfc5322NoWsFoldMailAddressPattern("([!#-'*+/-9=?A-Z^-~-]+(\\.[!#-'*+/-9=?A-Z^-~-]+)*|\"([]!#-[^-~ \t]|(\\[\t -~]))+\")@([!#-'*+/-9=?A-Z^-~-]+(\\.[!#-'*+/-9=?A-Z^-~-]+)*|\\[[\t -Z^-~]*])");

    QRegularExpression regExpSingleMailAddress("^"+rfc5322NoWsFoldMailAddressPattern+"$");
    QRegularExpressionMatch match = regExpSingleMailAddress.match(emailAddress);
    if (match.hasMatch())
        return true;

    QRegularExpression regexpMailAddressWithDisplayName("<"+rfc5322NoWsFoldMailAddressPattern+">$");
    match = regexpMailAddressWithDisplayName.match(emailAddress);
    return match.hasMatch();
}

QObject *EmailValidator::factory(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine);
    EmailValidator *validator = new EmailValidator(reinterpret_cast<QObject*>(engine));
    return validator;
}

static QWeakPointer<EmailValidator> validator;
QSharedPointer<EmailValidator> EmailValidator::instance()
{
    QSharedPointer<EmailValidator> val = validator.toStrongRef();
    if (val.isNull()) {
            val = QSharedPointer<EmailValidator>(new EmailValidator);
            validator = val;
    }
    return validator;
}
