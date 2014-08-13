/* This file is part of the KDE project
   Copyright (C) 2000 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "pi.h"
#include <kparts/part.h>
#include <kicon.h>
#include <kactioncollection.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kshell.h>
#include <kapplication.h>
#include <kpluginfactory.h>
#include <kauthorized.h>
#include <kio/netaccess.h>
#include <kparts/fileinfoextension.h>
#include <QRegExpValidator>

Pi::Pi( QObject* parent, const QVariantList & )
    : KParts::Plugin( parent )

{
    KAction *action = actionCollection()->addAction("calculatepi");
    action->setText(i18n( "&Calculate Pi..." ));
    connect(action, SIGNAL(triggered(bool)), SLOT(calcPi()));
}

void Pi::calcPi()
{
    KParts::ReadOnlyPart * part = qobject_cast<KParts::ReadOnlyPart *>(parent());
    //QRegExpValidator validator(QRegExp("[0-9]+"), part->widget());

    int count=0;
    bool ok;
    int total;
    /*
    qint64 total = KInputDialog::getText(i18n("Please enter loop total count"),
                                         i18n("Value of loop count"),
                                         i18n(""),
                                         &ok,
                                         part->widget(),
                                         &validator,
                                         i18n(""),
                                         i18n(""),
                                         QStringList("")).toLongLong();*/
    qreal pi=0;

    for (int i=0; i<total; i++) {
        qreal x = (qreal)qrand() / (RAND_MAX);
        qreal y = (qreal)qrand() / (RAND_MAX);

        if (x*x+y*y <= 1) {
            count++;
        }
    }

    pi = (qreal)count / (qreal)total * 4.0;

    KMessageBox::information(part->widget(),i18n("%1").arg(pi));
}

K_PLUGIN_FACTORY(KonqPiFactory, registerPlugin<Pi>();)
K_EXPORT_PLUGIN(KonqPiFactory("pi"))

#include "pi.moc"

