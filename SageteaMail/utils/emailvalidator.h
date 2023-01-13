/* Copyright (C) 2016 - 2017 Dan Chapman <dpniel@ubuntu.com>

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
#ifndef EMAILVALIDATOR_H
#define EMAILVALIDATOR_H

#include <QObject>
#include <QChar>
#include <QtQuick>
#include <QQmlEngine>
#include <QJSEngine>
#include <QDebug>
#include <QtGui/QGuiApplication>

class EmailValidator : public QObject
{
    Q_OBJECT
public:
    explicit EmailValidator(QObject *parent = 0);

    Q_INVOKABLE bool validate(const QString &emailAddress);

    // For qml singleton
    static QObject *factory(QQmlEngine *engine, QJSEngine *scriptEngine);
    // For c++ singleton
    static QSharedPointer<EmailValidator> instance();
};

#endif // EMAILVALIDATOR_H
